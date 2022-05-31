const functions = require("firebase-functions");

const admin = require('firebase-admin');
admin.initializeApp();

const express = require('express');
const cors = require('cors');

const app = express();

app.use(cors({origin: true}));

app.get('/status', (req, res)=>{
    return res.status(200).json({alive:true});
})

app.use(require('./routes/iot-api.route'));

exports.camgersem = functions.https.onRequest(app);