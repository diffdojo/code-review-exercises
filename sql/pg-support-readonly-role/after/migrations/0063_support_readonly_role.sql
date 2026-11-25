-- Read-only database access for the support team's Metabase.
CREATE ROLE support_readonly NOLOGIN;
GRANT USAGE ON SCHEMA public TO support_readonly;
GRANT SELECT ON ALL TABLES IN SCHEMA public TO support_readonly;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT ON TABLES TO support_readonly;

-- Metabase's login. Ops sets its password from the vault after the deploy.
CREATE ROLE metabase_support LOGIN CONNECTION LIMIT 5 IN ROLE support_readonly;
ALTER ROLE metabase_support SET default_transaction_read_only = on;
ALTER ROLE metabase_support SET statement_timeout = '30s';
