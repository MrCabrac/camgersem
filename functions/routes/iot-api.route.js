const { Router } = require('express');
const { firestore } = require('firebase-admin');
const functions = require('firebase-functions');
const router = Router();

const admin = require('firebase-admin');
const db = admin.firestore();

// Imports the Google Cloud client library
const { BigQuery } = require('@google-cloud/bigquery');

/**
 * {
 *  "name": <sensor-name> # "temperatura"
 *  "local_ip": <avris-platform-local-ip>
 *  "value": <sensor-value>
 * }
 */
router.route('/v1/sensors')
    .post((req, res) => {
        (async () => {
            try {
                console.log("Body: ", req.body);

                const doc = db.collection("sensores").doc(req.body.name);

                const item = await doc.get();
                if (item && item.exists) {
                    doc.update({
                        local_ip: req.body.local_ip,
                        time: firestore.Timestamp.fromDate(new Date()),
                        temperatura: req.body.temperatura,
                        humedad: req.body.humedad,
                        iluminacion: req.body.iluminacion
                        // values: firestore.FieldValue.arrayUnion([firestore.Timestamp.fromDate(new Date()), req.body.value]),
                        // values: firestore.FieldValue.arrayUnion(
                        //     {
                        //         time: firestore.Timestamp.fromDate(new Date()),
                        //         temperatura: req.body.temperatura,
                        //         humedad: req.body.humedad,
                        //         iluminacion: req.body.iluminacion
                        //     }),
                    }).then(() => {
                        return res.status(200).json({ status: "Success" });
                    }).catch((err) => {
                        console.log(err);
                        return res.status(500).json("Internal Server Error");
                    })
                }
            } catch (err) {
                return res.status(404).json("Not Found");
            }
        })();
    })

router.route('/v1/status')
    .get((req, res) => {
        (async () => {
            try {
                return res.status(200).json({ alive: true });
            } catch (err) {
                return res.status(500).send(err);
            }
        })();
    })

router.route('/v1/querier')
    .post((req, res) => {
        (async () => {
            try {
                console.log("Ahre BigQuery");

                // Creates a client
                const bigqueryClient = new BigQuery();
                // Rows to save
                const rows = [
                    { time: new Date(), temperatura: req.body.temperatura, humedad: req.body.humedad, iluminacion: req.body.iluminacion },
                ];
                // Insert data into a table
                await bigqueryClient
                    .dataset("datos_sensores")
                    .table("sensores")
                    .insert(rows).then(()=>{
                        return res.status(200).json({ status: `Inserted ${rows.length} rows` });
                    }).catch((err)=>{
                        return res.status(500).send(err);
                    });
            } catch (err) {
                return res.status(500).send(err);
            }
        })();
    })

router.route('/v1/sensors_data')
    .post((req, res) => {
        (async () => {
            try {
                console.log("Ahre BigQuery & Firestore");
                // Document referemce
                const doc = db.collection("sensores").doc(req.body.name);
                // Creates a client
                const bigqueryClient = new BigQuery();
                // Rows to save
                const rows = [
                    { time: new Date(), temperatura: req.body.temperatura, humedad: req.body.humedad, iluminacion: req.body.iluminacion },
                ];
                // Insert data into table
                await bigqueryClient
                    .dataset("datos_sensores")
                    .table("sensores")
                    .insert(rows).then(() => {
                        console.log(`Inserted ${rows.length} rows`);
                    }).catch((err) => {
                        console.log(`Error in bigQuery: `, err);
                    })

                const item = await doc.get();
                if (item && item.exists) {
                    doc.update({
                        local_ip: req.body.local_ip,
                        time: firestore.Timestamp.fromDate(new Date()),
                        temperatura: req.body.temperatura,
                        humedad: req.body.humedad,
                        iluminacion: req.body.iluminacion,
                        values: firestore.FieldValue.arrayUnion(
                            {
                                time: firestore.Timestamp.fromDate(new Date()),
                                temperatura: req.body.temperatura,
                                humedad: req.body.humedad,
                                iluminacion: req.body.iluminacion
                            })
                    }).then(() => {
                        return res.status(200).json({ status: "Success" });
                    }).catch((err) => {
                        console.log(err);
                        return res.status(500).json("Internal Server Error");
                    })
                }
            }
            catch (err) {
                return res.status(500).send(err);
            }
        })();
    })

module.exports = router;