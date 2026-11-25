const express = require("express");
const { listUsers } = require("../services/listUsers");
const { importUsers } = require("../services/importUsers");

const router = express.Router();

router.get("/users", listUsers);
router.post("/users/import", async (req, res) => {
  res.json(await importUsers(req.body.users));
});

module.exports = router;
