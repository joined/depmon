const express = require('express');
const app = express();
const port = 3000;

let currentStationId = null;

app.use(express.json());

app.get('/sysinfo', (req, res) => {
    res.send(
        JSON.stringify({
            version: 'Mock version',
            idf_version: 'Mock IDF version',
            project_name: 'Mock project name',
            compile_time: 'Mock compile time',
            compile_date: 'Mock compile date',
            mac_address: '1A:2B:3C:4D:5E:6F',
        })
    );
});

app.get('/currentstation', (req, res) => {
    res.send(
        JSON.stringify({
            id: currentStationId,
        })
    );
});

app.post('/currentstation', (req, res) => {
    // Fail 20% of the time
    if (Math.random() < 0.8) {
        currentStationId = req.body.id;
        res.send();
    } else {
        res.status(500).send();
    }
});

app.listen(port, () => {
    console.log(`Mock backend server listening on port ${port}`);
});
