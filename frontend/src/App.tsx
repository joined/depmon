import { h } from 'preact';
import { useState } from 'preact/hooks';
import { css } from '@emotion/css';
import Button from '@mui/material/Button';
import { useFetch } from 'usehooks-ts';
import { CircularProgress } from '@mui/material';

interface VersionResponse {
    version: string;
    idf_version: string;
    project_name: string;
    compile_time: string;
    compile_date: string;
}

const VersionInfo = () => {
    const { data, error } = useFetch<VersionResponse>('/api/version');

    if (error) {
        return <p>Error loading version information.</p>;
    }

    if (!data) {
        return <CircularProgress color="secondary" />;
    }

    return (
        <div>
            <p>
                <strong>Version:</strong> {data.version}
            </p>
            <p>
                <strong>Project name:</strong> {data.project_name}
            </p>
            <p>
                <strong>Compile time:</strong> {data.compile_time}
            </p>
            <p>
                <strong>Compile date:</strong> {data.compile_date}
            </p>
        </div>
    );
};

const App = () => {
    const [count, setCount] = useState(0);

    return (
        <div
            className={css`
                padding: 8px;
                color: white;
            `}>
            <h1
                className={css`
                    color: white;
                `}>
                Hello world in Preact
            </h1>
            <Button variant="contained" onClick={() => setCount((count) => count + 1)}>
                Hello world {count}
            </Button>
            <VersionInfo />
        </div>
    );
};

export default App;
