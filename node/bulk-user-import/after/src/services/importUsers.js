async function importUsers(users) {
  users.forEach(async (user) => {
    await db.insert(user);
  });
  console.log("All users imported ✅");
  return { imported: users.length };
}

module.exports = { importUsers };
