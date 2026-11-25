# Database login for the support team's Metabase

Database · PostgreSQL · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- The support team gets a database login for their Metabase so they can look customers up: through it they can read `customers`, `orders` and `tickets`.
- Support must not be able to read login credentials (`users`, which holds the password hashes), API tokens (`api_tokens`) or payment data (`payment_methods`). A table that a later migration adds stays unreadable for support unless that migration grants it to them explicitly.
- Support must not insert, update or delete rows in any table: the login gets no write privilege on any table.
- Privileges go to a group role `support_readonly` that cannot log in, and the Metabase login `metabase_support` is a member of it, so another support tool can later get its own login with the same access. The login allows at most 5 concurrent connections, defaults its sessions to read-only transactions and a 30-second `statement_timeout`.
- The login's password is not in the repository: ops sets it from the vault after the deploy.

## Pull request description

Gives the support team's Metabase its own Postgres login. `migrations/0063_support_readonly_role.sql` (new) creates the `support_readonly` group role (no login), lets it use the `public` schema and grants it read access, then creates the `metabase_support` login as a member of that role with a 5-connection limit, read-only transactions by default and a 30-second statement timeout. The password is set by ops from the vault, so the migration does not contain it.

## Context

Shop backend: PostgreSQL 16, one primary; the API is Python 3.12 with psycopg 3 (plain SQL). Every application table lives in schema `public`, created by migration `0001` and unchanged: `users` (customers' logins: `id`, `email`, `password_hash` (argon2id), `created_at`), `api_tokens` (`id`, `user_id`, `token_hash bytea`, `created_at`), `customers` (`id`, `user_id`, `name`, `email`, `plan`), `orders` (`id`, `customer_id`, `total_cents`, `status`, `created_at`), `tickets` (`id`, `customer_id`, `subject`, `status`, `created_at`) and `payment_methods` (`id`, `customer_id`, `brand`, `last4`, `exp_month`, `exp_year`, `provider_token`, the payment provider's reusable token for the card). Besides those six the schema holds only the runner's `schema_migrations`, no views, sequences other than the identity columns' own, or functions. Roles: the API and the migration runner both connect as `app` (LOGIN, CREATEROLE, not a superuser); `app` owns schema `public` and created every table in it, and it is the only role that creates tables. Before this PR no other role has any privilege on these tables; the database's default PUBLIC privileges (CONNECT, TEMP) are unchanged and out of scope. Migrations are plain SQL files in `migrations/`, applied in name order by the existing runner `app/migrate.py` at the start of every deploy, each file in its own transaction, and recorded in `schema_migrations`. Metabase runs on the internal network and connects to the primary; `pg_hba.conf` (ops-managed, unchanged) admits `metabase_support` only from the Metabase host with scram-sha-256, and Metabase's own pool for this database is capped at 5 connections. Support staff sign in to Metabase, not to Postgres, and Metabase's native SQL editor is enabled for them.

## Diff

`migrations/0063_support_readonly_role.sql` (new file)

```diff
--- /dev/null
+++ b/migrations/0063_support_readonly_role.sql
@@ -0,0 +1,10 @@
+-- Read-only database access for the support team's Metabase.
+CREATE ROLE support_readonly NOLOGIN;
+GRANT USAGE ON SCHEMA public TO support_readonly;
+GRANT SELECT ON ALL TABLES IN SCHEMA public TO support_readonly;
+ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT ON TABLES TO support_readonly;
+
+-- Metabase's login. Ops sets its password from the vault after the deploy.
+CREATE ROLE metabase_support LOGIN CONNECTION LIMIT 5 IN ROLE support_readonly;
+ALTER ROLE metabase_support SET default_transaction_read_only = on;
+ALTER ROLE metabase_support SET statement_timeout = '30s';
```

The full files are in [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Database login for the support team's Metabase](https://app.diffdojo.com/challenges/pg-support-readonly-role?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.
