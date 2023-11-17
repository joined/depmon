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
                      tasks: [
                          {
                              name: 'TaskWithPrettyLongName',
                              priority: 1,
                              state: 1,
                              stack_high_water_mark: 123456,
                              runtime: enableRuntime ? 12 : null,
                              core_id: enableCoreId ? 0 : null,
                          },
                          {
                              name: 'task2',
                              priority: 2,
                              state: 2,
                              stack_high_water_mark: 23456,
                              runtime: enableRuntime ? 23 : null,
                              core_id: enableCoreId ? 1 : null,
                          },
                      ],
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
