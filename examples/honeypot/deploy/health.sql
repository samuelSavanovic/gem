-- The honeypot's health by UTC day, against PLAN.md's "Success criteria":
--   sqlite3 -box /var/lib/honeypot/honeypot-2026-10-12.db < health.sql
-- One weekly file at a time; a 7-day window spans at most two.
--
-- fds_extra and res_extra are the fds and open resources beyond one per
-- live session. They don't depend on the traffic (9 and 2 in the soaks),
-- so any rise is a leak.
WITH m AS (
  SELECT *, date(time_ms / 1000, 'unixepoch') AS day,
         uptime_ms < lag(uptime_ms) OVER (ORDER BY time_ms) AS restarted
  FROM metrics
)
SELECT day,
       count(*) AS samples,
       sum(restarted) AS restarts,
       min(rss_kb) / 1024 AS rss_min_mb,
       max(rss_kb) / 1024 AS rss_max_mb,
       max(fds - sessions) AS fds_extra,
       max(resources_open - sessions) AS res_extra,
       max(resources_ownerless) AS ownerless,
       max(sessions) AS peak_sessions,
       sum(resets_over_100ms) AS resets_100ms,
       sum(events_dropped) AS dropped,
       sum(spawn_refused) AS spawn_refused,
       (SELECT count(*) FROM crashes
         WHERE date(crashes.time_ms / 1000, 'unixepoch') = m.day) AS crashes,
       (SELECT count(*) FROM sessions
         WHERE end_reason = 'lost' AND date(sessions.end_ms / 1000, 'unixepoch') = m.day) AS lost
FROM m
GROUP BY day
ORDER BY day;
