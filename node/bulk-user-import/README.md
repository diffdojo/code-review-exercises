# Bulk import users

Node · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- Add an admin endpoint that imports a list of users.
- Every user must be inserted into the database.
- The endpoint must only report success after all inserts have completed.
- A failed insert must not be silently swallowed.

## Pull request description

Implements bulk user import. Adds an `importUsers` service that inserts each user with `async`/`await`, and wires it to a new admin route. Verified with a small test file — all rows appeared in the table.

## Context

Runs in an admin-triggered batch job; imports can be several thousand users.

## Diff

`src/services/importUsers.js` (new file)

```diff
--- /dev/null
+++ b/src/services/importUsers.js
@@ -0,0 +1,9 @@
+async function importUsers(users) {
+  users.forEach(async (user) => {
+    await db.insert(user);
+  });
+  console.log("All users imported ✅");
+  return { imported: users.length };
+}
+
+module.exports = { importUsers };
```

`src/routes/admin.js` (modified)

```diff
--- a/src/routes/admin.js
+++ b/src/routes/admin.js
@@ -1,8 +1,12 @@
 const express = require("express");
 const { listUsers } = require("../services/listUsers");
+const { importUsers } = require("../services/importUsers");
 
 const router = express.Router();
 
 router.get("/users", listUsers);
+router.post("/users/import", async (req, res) => {
+  res.json(await importUsers(req.body.users));
+});
 
 module.exports = router;
```

The full files are in [`before/`](before) and [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Bulk import users](https://app.diffdojo.com/challenges/bulk-user-import?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.

A written walkthrough of this PR, answer included: https://diffdojo.com/blog/async-foreach-import-review
