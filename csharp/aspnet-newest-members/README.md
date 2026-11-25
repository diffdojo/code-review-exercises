# Newest members card on the workspace home page

C# · ASP.NET Core · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- New endpoint `GET /api/workspaces/{workspaceId}/members/newest` for the "New members" card on the workspace home page. It returns the workspace's 20 most recently joined members as a JSON array, newest first: the member with the latest `JoinedAt` comes first. Two members with the same `JoinedAt` are ordered by `Id`, higher `Id` first.
- A workspace with fewer than 20 members returns all of them, in the same newest-first order.
- Each item has `id`, `userId`, `displayName` and `joinedAt` (the membership's `JoinedAt`, UTC). Only members of the workspace in the path are returned, and no other membership field (such as `Email`) is included.
- Only members of the workspace may call it: the existing `WorkspaceMember` authorization policy answers 401 or 403 itself. A `workspaceId` that is not an integer does not match the route and answers 404, like the other workspace routes.

## Pull request description

Adds the data behind the new "New members" card on the workspace home page. `Members/NewestMembersEndpoint.cs` maps `GET /api/workspaces/{workspaceId}/members/newest` behind the `WorkspaceMember` policy and reads the workspace's memberships with one EF Core query, capped at `Limit` (20) and projected straight into `NewestMemberDto`, so `Email` and the other membership columns never leave the database. `Program.cs` registers the endpoint next to the existing workspace routes.

## Context

ASP.NET Core 10 minimal APIs on .NET 10 (C# 14, nullable reference types and implicit usings enabled), EF Core 10 with Npgsql on PostgreSQL 16. `AppDbContext` (existing, unchanged) has `DbSet<Membership> Memberships`; `Membership` has `int Id` (identity), `int WorkspaceId`, `int UserId`, `string DisplayName`, `string Email` and `DateTime JoinedAt`, stored as `timestamptz` and read back as UTC. A membership row exists only while the user is a member (leaving deletes it), a workspace has at most 5,000 members, and there is an index on `(WorkspaceId, JoinedAt)`. `AddWorkspaceAuth` and the `WorkspaceMember` policy (existing, unchanged) sign the user in from the session cookie and succeed only when the signed-in user has a membership in the `workspaceId` route value; otherwise the request gets 401 (not signed in) or 403 before the handler runs. `MapWorkspaces`, the exception handler and `AddProblemDetails` are unchanged; a database error surfaces as the usual 500 problem response. The web app's card ships separately and shows the items in the order the endpoint returns them. The API tests (xUnit + `WebApplicationFactory`) cover the existing workspace routes; this PR adds none.

## Diff

`src/Workspaces.Api/Program.cs` (modified)

```diff
--- a/src/Workspaces.Api/Program.cs
+++ b/src/Workspaces.Api/Program.cs
@@ -1,6 +1,7 @@
 using Microsoft.EntityFrameworkCore;
 using Workspaces.Api.Auth;
 using Workspaces.Api.Data;
+using Workspaces.Api.Members;
 using Workspaces.Api.Workspaces;
 
 var builder = WebApplication.CreateBuilder(args);
@@ -17,6 +18,7 @@
 app.UseAuthorization();
 
 app.MapWorkspaces();
+app.MapNewestMembers();
 
 app.Run();
 
```

`src/Workspaces.Api/Members/NewestMembersEndpoint.cs` (new file)

```diff
--- /dev/null
+++ b/src/Workspaces.Api/Members/NewestMembersEndpoint.cs
@@ -0,0 +1,31 @@
+using Microsoft.EntityFrameworkCore;
+using Workspaces.Api.Data;
+
+namespace Workspaces.Api.Members;
+
+public static class NewestMembersEndpoint
+{
+    public const int Limit = 20;
+
+    public static IEndpointRouteBuilder MapNewestMembers(this IEndpointRouteBuilder app)
+    {
+        app.MapGet("/api/workspaces/{workspaceId:int}/members/newest", GetNewestAsync)
+            .RequireAuthorization("WorkspaceMember");
+        return app;
+    }
+
+    private static async Task<List<NewestMemberDto>> GetNewestAsync(
+        int workspaceId, AppDbContext db, CancellationToken ct)
+    {
+        return await db.Memberships
+            .AsNoTracking()
+            .Where(m => m.WorkspaceId == workspaceId)
+            .OrderBy(m => m.JoinedAt)
+            .ThenBy(m => m.Id)
+            .Take(Limit)
+            .Select(m => new NewestMemberDto(m.Id, m.UserId, m.DisplayName, m.JoinedAt))
+            .ToListAsync(ct);
+    }
+}
+
+public sealed record NewestMemberDto(int Id, int UserId, string DisplayName, DateTime JoinedAt);
```

The full files are in [`before/`](before) and [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Newest members card on the workspace home page](https://app.diffdojo.com/challenges/aspnet-newest-members?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.
