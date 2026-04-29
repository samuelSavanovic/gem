import http from "node:http";
import fs from "node:fs";
import Database from "better-sqlite3";

const PORT = 8081;

// --- Database setup ---
try { fs.unlinkSync("bookmarks.db"); } catch {}
const db = new Database("bookmarks.db");
db.pragma("journal_mode = WAL");
db.exec(`
  CREATE TABLE IF NOT EXISTS bookmarks (
    id INTEGER PRIMARY KEY,
    url TEXT NOT NULL,
    title TEXT NOT NULL,
    tags TEXT,
    created_at INTEGER
  )
`);

const stmtAll = db.prepare("SELECT * FROM bookmarks ORDER BY created_at DESC");
const stmtInsert = db.prepare("INSERT INTO bookmarks (url, title, tags, created_at) VALUES (?, ?, ?, ?)");

// --- HTML helpers ---
function esc(s) {
  if (s == null) return "";
  return String(s).replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#39;");
}

function formatDate(ms) {
  if (ms == null) return "";
  const d = new Date(ms);
  const pad = (n) => String(n).padStart(2, "0");
  return `${d.getUTCFullYear()}-${pad(d.getUTCMonth() + 1)}-${pad(d.getUTCDate())} ${pad(d.getUTCHours())}:${pad(d.getUTCMinutes())}`;
}

function renderBookmarkRow(b) {
  return `<tr id='bookmark-${b.id}'>
<td><a href='${esc(b.url)}' target='_blank'>${esc(b.url)}</a></td>
<td id='title-${b.id}'>${esc(b.title)}</td>
<td>${esc(b.tags)}</td>
<td>${formatDate(b.created_at)}</td>
<td class='actions'>
<button hx-post='/bookmarks/${b.id}/fetch-title' hx-target='#title-${b.id}' hx-swap='innerHTML'>Fetch Title</button>
 <button hx-get='/bookmarks/${b.id}/edit' hx-target='#bookmarks' hx-swap='innerHTML'>Edit</button>
 <button hx-delete='/bookmarks/${b.id}' hx-target='#bookmarks' hx-swap='innerHTML' hx-confirm='Delete this bookmark?'>Delete</button>
</td>
</tr>`;
}

function renderBookmarkList(bookmarks) {
  let parts = ["<table><thead><tr><th>URL</th><th>Title</th><th>Tags</th><th>Created</th><th>Actions</th></tr></thead><tbody>"];
  for (const b of bookmarks) {
    parts.push(renderBookmarkRow(b));
  }
  parts.push("</tbody></table>");
  if (bookmarks.length === 0) {
    parts.push("<p class='empty'>No bookmarks yet. Add one above!</p>");
  }
  return parts.join("");
}

// --- Static file cache ---
import { fileURLToPath } from "node:url";
import path from "node:path";
const __dirname = path.dirname(fileURLToPath(import.meta.url));
const indexHtml = fs.readFileSync(path.join(__dirname, "../../examples/bookmark_app/static/index.html"), "utf8");

// --- URL-encoded form parser ---
function parseForm(body) {
  const params = {};
  for (const pair of body.split("&")) {
    const [k, ...rest] = pair.split("=");
    params[decodeURIComponent(k)] = decodeURIComponent(rest.join("=").replace(/\+/g, " "));
  }
  return params;
}

// --- Request handler ---
const server = http.createServer((req, res) => {
  if (req.method === "GET" && req.url === "/") {
    res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
    res.end(indexHtml);
    return;
  }

  if (req.method === "GET" && req.url === "/bookmarks") {
    const bookmarks = stmtAll.all();
    const html = renderBookmarkList(bookmarks);
    res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
    res.end(html);
    return;
  }

  if (req.method === "POST" && req.url === "/bookmarks") {
    let body = "";
    req.on("data", (chunk) => { body += chunk; });
    req.on("end", () => {
      const form = parseForm(body);
      const url = form.url || "";
      let title = form.title || url;
      const tags = form.tags || "";
      if (!url) {
        res.writeHead(400, { "Content-Type": "text/plain" });
        res.end("URL is required");
        return;
      }
      stmtInsert.run(url, title, tags, Date.now());
      const bookmarks = stmtAll.all();
      const html = renderBookmarkList(bookmarks);
      res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
      res.end(html);
    });
    return;
  }

  res.writeHead(404, { "Content-Type": "text/plain" });
  res.end("Not Found");
});

server.listen(PORT, () => {
  console.log(`Node.js bookmark server on http://localhost:${PORT}`);
});
