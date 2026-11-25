# Skip repeated addresses in contact imports

Java · Spring Boot · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- `ContactImportService.importEmails(listId, emails)` receives the email column of an uploaded contacts file as a non-null list in file order. A file holds up to 200,000 rows; every email is a non-null, non-empty ASCII string of at most 254 characters.
- The same address can appear more than once in a file, in different letter case or with surrounding whitespace. Two rows are the same contact when their emails are equal after stripping surrounding whitespace and lower-casing. Store each contact once, the first occurrence in file order, in that normalised form, and count every other occurrence as a skipped duplicate.
- The upload response is `{"imported": n, "duplicatesSkipped": m}`: `n` contacts written, `m` rows skipped.
- The whole import runs inside the upload request, which the API gateway cuts off after 10 seconds. Writing 200,000 contacts takes about 2 seconds, so the deduplication must run in time linear in the number of rows; a 200,000-row file must finish well inside the budget.

## Pull request description

Imports of a file that repeats an address failed on the unique index on `(list_id, email)`, so the whole upload was rejected. This dedupes the rows before writing: each email is normalised with `strip().toLowerCase(Locale.ROOT)`, the first occurrence is kept in file order, and repeats are counted and returned as `duplicatesSkipped`.

## Context

Mailing-list backend on Spring Boot 3.3 (Java 21) with PostgreSQL 16. `ImportController` (existing, unchanged) handles `POST /api/lists/{listId}/imports`: it creates the new list, parses the CSV (rejecting blank rows and files over 200,000 rows), calls `importEmails` and returns the `ImportResult` as JSON; nothing else constructs an `ImportResult`. Every import targets a freshly created list, so no contacts exist in it beforehand. `ContactWriter.insertAll(listId, emails)` (existing, unchanged) writes the given emails with JDBC batch inserts of 1,000 rows in one transaction; `contacts` has a unique index on `(list_id, email)`. Customers upload exports from other mailing tools; files of 100,000 to 200,000 rows are common, and repeats are typically a few percent of rows.

## Diff

`src/main/java/com/acme/mailing/imports/ContactImportService.java` (modified)

```diff
--- a/src/main/java/com/acme/mailing/imports/ContactImportService.java
+++ b/src/main/java/com/acme/mailing/imports/ContactImportService.java
@@ -19,13 +19,19 @@
     }
 
     public ImportResult importEmails(long listId, List<String> emails) {
-        List<String> normalized = new ArrayList<>(emails.size());
+        List<String> unique = new ArrayList<>(emails.size());
+        int duplicates = 0;
         for (String email : emails) {
-            normalized.add(normalize(email));
+            String normalized = normalize(email);
+            if (unique.contains(normalized)) {
+                duplicates++;
+            } else {
+                unique.add(normalized);
+            }
         }
-        writer.insertAll(listId, normalized);
-        log.info("Imported {} contacts into list {}", normalized.size(), listId);
-        return new ImportResult(normalized.size());
+        writer.insertAll(listId, unique);
+        log.info("Imported {} contacts into list {}, skipped {} duplicate rows", unique.size(), listId, duplicates);
+        return new ImportResult(unique.size(), duplicates);
     }
 
     static String normalize(String email) {
```

`src/main/java/com/acme/mailing/imports/ImportResult.java` (modified)

```diff
--- a/src/main/java/com/acme/mailing/imports/ImportResult.java
+++ b/src/main/java/com/acme/mailing/imports/ImportResult.java
@@ -1,4 +1,4 @@
 package com.acme.mailing.imports;
 
-public record ImportResult(int imported) {
+public record ImportResult(int imported, int duplicatesSkipped) {
 }
```

The full files are in [`before/`](before) and [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Skip repeated addresses in contact imports](https://app.diffdojo.com/challenges/spring-contact-import-dedupe?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.
