-- The server's health by UTC day, against PLAN.md's "Success criteria":
--   sqlite3 -box /var/lib/httpd/httpd-2026-10-12.db < health.sql
-- One weekly file at a time; a 7-day window spans at most two.
--
-- fds_extra and res_extra are the fds and open resources beyond one per
-- open connection; fds_extra is the day's median, since one sample's
-- swings with the connections that open and close around it (as the
-- honeypot's, examples/honeypot/deploy/health.sql). Any rise in either
-- from day to day is a leak.
WITH m AS (
  SELECT *, date(time_ms / 1000, 'unixepoch') AS day,
         fds - connections AS fx,
         uptime_ms < lag(uptime_ms) OVER (ORDER BY time_ms) AS restarted
  FROM metrics
),
ranked AS (
  SELECT day, fx,
         row_number() OVER (PARTITION BY day ORDER BY fx) AS rn,
         count(*) OVER (PARTITION BY day) AS n
  FROM m
),
fds_median AS (
  SELECT day, avg(fx) AS fds_extra
  FROM ranked
  WHERE rn IN ((n + 1) / 2, (n + 2) / 2)
  GROUP BY day
)
SELECT m.day,
       count(*) AS samples,
       sum(restarted) AS restarts,
       min(rss_kb) / 1024 AS rss_min_mb,
       max(rss_kb) / 1024 AS rss_max_mb,
       fds_median.fds_extra,
       max(resources_open - connections) AS res_extra,
       max(resources_ownerless) AS ownerless,
       max(connections) AS peak_conns,
       sum(resets_over_100ms) AS resets_100ms,
       sum(events_dropped) AS dropped,
       sum(spawn_refused) AS spawn_refused,
       (SELECT count(*) FROM requests
         WHERE date(requests.time_ms / 1000, 'unixepoch') = m.day) AS requests,
       (SELECT count(*) FROM bad_input
         WHERE date(bad_input.time_ms / 1000, 'unixepoch') = m.day) AS bad_input,
       (SELECT count(*) FROM cap_events
         WHERE date(cap_events.time_ms / 1000, 'unixepoch') = m.day) AS caps,
       (SELECT count(*) FROM crashes
         WHERE date(crashes.time_ms / 1000, 'unixepoch') = m.day) AS crashes,
       (SELECT count(*) FROM connections
         WHERE end_reason = 'lost' AND date(connections.end_ms / 1000, 'unixepoch') = m.day) AS lost
FROM m JOIN fds_median USING (day)
GROUP BY m.day
ORDER BY m.day;
