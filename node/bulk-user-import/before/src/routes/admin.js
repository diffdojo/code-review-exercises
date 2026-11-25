const express = require("express");
const { listUsers } = require("../services/listUsers");

const router = express.Router();

router.get("/users", listUsers);

module.exports = router;
