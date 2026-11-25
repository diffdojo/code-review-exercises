using Microsoft.EntityFrameworkCore;
using Workspaces.Api.Auth;
using Workspaces.Api.Data;
using Workspaces.Api.Members;
using Workspaces.Api.Workspaces;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseNpgsql(builder.Configuration.GetConnectionString("Default")));
builder.Services.AddWorkspaceAuth(builder.Configuration);
builder.Services.AddProblemDetails();

var app = builder.Build();

app.UseExceptionHandler();
app.UseAuthentication();
app.UseAuthorization();

app.MapWorkspaces();
app.MapNewestMembers();

app.Run();

public partial class Program;
