#!/bin/bash
# Smoke test for the STOMP broker. Spawns two subscribers and a publisher,
# verifies fan-out on a topic, then verifies round-robin on a queue.
#
# Assumes a broker running on localhost:61613.
#
# Usage: ./smoke_test.sh [port]

set -uo pipefail

PORT="${1:-61613}"
TMP="$(mktemp -d)"
trap "rm -rf $TMP" EXIT

# Helper: send a STOMP frame followed by listening for $1 seconds.
# All frames should be in stdin.
stomp_session() {
    local outfile="$1"
    local timeout="$2"
    nc -w "$timeout" localhost "$PORT" > "$outfile"
}

# ─── Test 1: topic fan-out ─────────────────────────────────────
echo "=== test 1: topic fan-out (2 subscribers) ==="

# Subscriber 1
{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SUBSCRIBE\nid:s1\ndestination:/topic/news\n\n\0'
    sleep 4
} | stomp_session "$TMP/s1.out" 5 &
S1_PID=$!

# Subscriber 2
{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SUBSCRIBE\nid:s2\ndestination:/topic/news\n\n\0'
    sleep 4
} | stomp_session "$TMP/s2.out" 5 &
S2_PID=$!

# Give subscribers a moment to land their SUBSCRIBE
sleep 1

# Publisher
{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SEND\ndestination:/topic/news\ncontent-type:text/plain\n\nbreaking news\0'
    sleep 1
} | nc -w 2 localhost "$PORT" > "$TMP/p1.out"

wait $S1_PID $S2_PID

echo "--- subscriber 1 output ---"
cat -v "$TMP/s1.out" | head -20
echo "--- subscriber 2 output ---"
cat -v "$TMP/s2.out" | head -20

if grep -q "breaking news" "$TMP/s1.out" && grep -q "breaking news" "$TMP/s2.out"; then
    echo "PASS: both subscribers received MESSAGE"
else
    echo "FAIL: at least one subscriber missed the MESSAGE"
fi

# ─── Test 2: queue round-robin ─────────────────────────────────
echo ""
echo "=== test 2: queue round-robin (2 consumers) ==="

# Consumer 1
{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SUBSCRIBE\nid:c1\ndestination:/queue/work\n\n\0'
    sleep 4
} | stomp_session "$TMP/c1.out" 5 &
C1_PID=$!

# Consumer 2
{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SUBSCRIBE\nid:c2\ndestination:/queue/work\n\n\0'
    sleep 4
} | stomp_session "$TMP/c2.out" 5 &
C2_PID=$!

sleep 1

# Publisher sends 4 messages
{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    for i in 1 2 3 4; do
        printf "SEND\ndestination:/queue/work\n\njob-${i}\0"
    done
    sleep 1
} | nc -w 2 localhost "$PORT" > "$TMP/p2.out"

wait $C1_PID $C2_PID

echo "--- consumer 1 output ---"
cat -v "$TMP/c1.out" | head -20
echo "--- consumer 2 output ---"
cat -v "$TMP/c2.out" | head -20

# Each consumer should have received roughly half the jobs (round-robin).
C1_COUNT=$(grep -c "job-" "$TMP/c1.out" || true)
C2_COUNT=$(grep -c "job-" "$TMP/c2.out" || true)
echo "consumer 1 got $C1_COUNT, consumer 2 got $C2_COUNT (total should be 4)"
if [ "$((C1_COUNT + C2_COUNT))" -eq 4 ] && [ "$C1_COUNT" -gt 0 ] && [ "$C2_COUNT" -gt 0 ]; then
    echo "PASS: queue round-robin distributed all 4 messages, both consumers got at least one"
else
    echo "FAIL: queue distribution unexpected"
fi

# ─── Test 3: adversarial — garbage in, broker survives ─────────
echo ""
echo "=== test 3: garbage frame doesn't take down the broker ==="

# Garbage client
printf 'GARBAGE_NOT_A_FRAME_NO_NUL_OR_HEADERS' | nc -w 2 localhost "$PORT" > "$TMP/g.out"

# Now make sure the broker still works — connect a fresh subscriber/publisher
{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SUBSCRIBE\nid:s3\ndestination:/topic/post-garbage\n\n\0'
    sleep 3
} | nc -w 4 localhost "$PORT" > "$TMP/post1.out" &
PG_SUB_PID=$!

sleep 1

{
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SEND\ndestination:/topic/post-garbage\n\nstill alive\0'
    sleep 1
} | nc -w 2 localhost "$PORT" > "$TMP/post2.out"

wait $PG_SUB_PID

if grep -q "still alive" "$TMP/post1.out"; then
    echo "PASS: broker still serving after garbage input"
else
    echo "FAIL: broker did not deliver post-garbage MESSAGE"
    echo "--- post1 ---"
    cat -v "$TMP/post1.out"
fi

# ─── Test 4: connection death prunes subscriber ────────────────
echo ""
echo "=== test 4: dropped subscriber is pruned (no leak) ==="

# Subscribe a "ghost" — connect, subscribe, then close socket (no DISCONNECT).
# `nc` with stdin EOF closes immediately. The destination should monitor the
# connection and prune on DOWN. We don't have direct introspection, but we
# verify by: (a) sending after the ghost dies, (b) confirming no error and
# the publisher still completes cleanly.

(
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SUBSCRIBE\nid:ghost\ndestination:/topic/ghost\n\n\0'
) | nc -w 1 localhost "$PORT" > /dev/null
sleep 1  # let DOWN propagate

# Send to /topic/ghost — should not error even though no live subscriber
RESULT=$({
    printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0'
    printf 'SEND\ndestination:/topic/ghost\n\nhello ghost\0'
    sleep 1
} | nc -w 2 localhost "$PORT")

if echo "$RESULT" | grep -q "CONNECTED"; then
    echo "PASS: broker accepted SEND to topic with no live subscribers"
else
    echo "FAIL: broker errored on SEND to ghost-only topic"
fi
