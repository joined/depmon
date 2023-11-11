import { h } from 'preact';
import { css } from '@emotion/css';
import { useFetch } from 'usehooks-ts';
import { CircularProgress, CssBaseline, ThemeProvider, createTheme } from '@mui/material';

const darkTheme = createTheme({
    palette: {
        mode: 'dark',
    },
});

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
        <ul>
            <li>
                <strong>Version:</strong> {data.version}
            </li>
            <li>
                <strong>ESP-IDF version:</strong> {data.idf_version}
            </li>
            <li>
                <strong>Project name:</strong> {data.project_name}
            </li>
            <li>
                <strong>Compile time:</strong> {data.compile_time}
            </li>
            <li>
                <strong>Compile date:</strong> {data.compile_date}
            </li>
        </ul>
    );
};

const App = () => {
    return (
        <ThemeProvider theme={darkTheme}>
            <CssBaseline />
            <main
                className={css`
                    padding: 8px;
                    color: white;
                    max-width: 600px;
                    margin: 0 auto;
                `}>
                <VersionInfo />
            </main>
        </ThemeProvider>
    );
};

export default App;
