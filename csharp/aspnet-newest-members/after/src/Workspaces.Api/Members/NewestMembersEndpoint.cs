using Microsoft.EntityFrameworkCore;
using Workspaces.Api.Data;

namespace Workspaces.Api.Members;

public static class NewestMembersEndpoint
{
    public const int Limit = 20;

    public static IEndpointRouteBuilder MapNewestMembers(this IEndpointRouteBuilder app)
    {
        app.MapGet("/api/workspaces/{workspaceId:int}/members/newest", GetNewestAsync)
            .RequireAuthorization("WorkspaceMember");
        return app;
    }

    private static async Task<List<NewestMemberDto>> GetNewestAsync(
        int workspaceId, AppDbContext db, CancellationToken ct)
    {
        return await db.Memberships
            .AsNoTracking()
            .Where(m => m.WorkspaceId == workspaceId)
            .OrderBy(m => m.JoinedAt)
            .ThenBy(m => m.Id)
            .Take(Limit)
            .Select(m => new NewestMemberDto(m.Id, m.UserId, m.DisplayName, m.JoinedAt))
            .ToListAsync(ct);
    }
}

public sealed record NewestMemberDto(int Id, int UserId, string DisplayName, DateTime JoinedAt);
