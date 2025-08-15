'use strict';
const path = require('path');
const nodeGypBuild = require('node-gyp-build');
module.exports = nodeGypBuild(path.join(__dirname, '..'));