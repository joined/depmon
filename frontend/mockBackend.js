const express = require('express');
const app = express();
const port = 3000;

app.get('/version', (req, res) => {
    res.send(
        JSON.stringify({
            version: 'Mock version',
            idf_version: 'Mock IDF version',
            project_name: 'Mock project name',
            compile_time: 'Mock compile time',
            compile_date: 'Mock compile date',
        })
    );
});

app.listen(port, () => {
    console.log(`Mock backend server listening on port ${port}`);
});
