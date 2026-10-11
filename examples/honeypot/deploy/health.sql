-- The honeypot's health by UTC day, against PLAN.md's "Success criteria":
--   sqlite3 -box /var/lib/honeypot/honeypot-2026-10-12.db < health.sql
-- One weekly file at a time; a 7-day window spans at most two.
--
-- fds_extra and res_extra are the fds and open resources beyond one per
-- live session. res_extra stayed at 2 in every soak sample, so any rise is
-- a leak. fds_extra is the day's median: one sample's ranged from 4 to 17
-- in a soak at the default load (about 5,000 sessions) while its median
-- stayed at 9, so the maximum would not tell a leak from the traffic.
WITH m AS (
  SELECT *, date(time_ms / 1000, 'unixepoch') AS day,
         fds - sessions AS fx,
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
FROM m JOIN fds_median USING (day)
GROUP BY m.day
ORDER BY m.day;
