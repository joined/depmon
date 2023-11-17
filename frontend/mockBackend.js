const express = require('express');
const app = express();
const port = 3000;

let currentStationId = null;

app.use(express.json());

app.get('/sysinfo', (req, res) => {
    const enableTrace = true;
    const enableRuntime = true;
    const enableCoreId = true;
    res.send(
        JSON.stringify({
            software: {
                version: 'Mock version',
                idf_version: 'Mock IDF version',
                project_name: 'Mock project name',
                compile_time: 'Mock compile time',
                compile_date: 'Mock compile date',
            },
            hardware: {
                mac_address: '1A:2B:3C:4D:5E:6F',
            },
            memory: {
                free_heap: 123456,
                minimum_free_heap: 123456,
            },
            ...(enableTrace
                ? {
                      tasks: [...Array(10)].map((_, index) => ({
                          name: `TaskNumber${index}`,
                          priority: index,
                          state: Math.floor(Math.random() * 6),
                          stack_high_water_mark: Math.floor(Math.random() * 10000),
                          runtime: enableRuntime ? Math.floor(Math.random() * 100) : null,
                          core_id: enableCoreId ? Math.floor(Math.random() * 2) : null,
                      })),
                  }
                : null),
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
