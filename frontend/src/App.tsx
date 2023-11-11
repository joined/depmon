import { h } from 'preact';
import { css } from '@emotion/css';
import { useFetch } from 'usehooks-ts';
import { Station, stations } from './stations';
import {
    Autocomplete,
    Button,
    CircularProgress,
    CssBaseline,
    Stack,
    TextField,
    ThemeProvider,
    createFilterOptions,
    createTheme,
} from '@mui/material';
import parse from 'autosuggest-highlight/parse';
import match from 'autosuggest-highlight/match';
import { useEffect, useState } from 'preact/compat';

const theme = createTheme({
    palette: {
        mode: 'dark',
    },
});

export function StationSelector() {
    const filterOptions = createFilterOptions<Station>({
        ignoreCase: true,
        limit: 10,
    });

    const { data, error } = useFetch<{ id: string }>('/api/currentstation');

    const [selectedStation, setSelectedStation] = useState<Station | null>(null);
    const [inputValue, setInputValue] = useState('');

    useEffect(() => {
        if (data) {
            setSelectedStation(stations.find((s) => s.id === data.id) || null);
        }
    }, [data]);

    const onSubmit = async (event: h.JSX.TargetedEvent<HTMLFormElement>) => {
        event.preventDefault();

        try {
            const response = await fetch('/api/currentstation', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ id: selectedStation!.id }),
            });

            if (!response.ok) {
                // TODO If the request fail, we don't know if station was updated or not.
                // We should handle that better.
                throw new Error('Network response was not ok');
            }

            // Handle success, e.g., show a success message or redirect
        } catch (error) {
            console.error('There was a problem with the fetch operation:', error);
            // Handle error, e.g., show an error message
        }
    };

    if (!data) {
        return <CircularProgress color="secondary" />;
    }

    if (error) {
        return <p>Error loading station information.</p>;
    }

    return (
        <form onSubmit={onSubmit}>
            <Stack direction="row" spacing={2}>
                <Autocomplete<Station>
                    disablePortal
                    id="station-selector"
                    options={stations}
                    filterOptions={filterOptions}
                    inputValue={inputValue}
                    onInputChange={(event, newInputValue) => {
                        setInputValue(newInputValue);
                    }}
                    value={selectedStation}
                    onChange={(event: any, newStation: Station | null) => {
                        setSelectedStation(newStation);
                    }}
                    sx={{ width: 300 }}
                    renderInput={(params) => <TextField {...params} label="Station" />}
                    getOptionLabel={(option) => option.name}
                    renderOption={(props, option, { inputValue }) => {
                        const matches = match(option.name, inputValue, { insideWords: true });
                        const parts = parse(option.name, matches);

                        return (
                            <li {...props}>
                                <div>
                                    {parts.map((part, index) => (
                                        <span
                                            key={index}
                                            style={{
                                                fontWeight: part.highlight ? 700 : 400,
                                            }}>
                                            {part.text}
                                        </span>
                                    ))}
                                </div>
                            </li>
                        );
                    }}
                />
                <Button type="submit" variant="contained" disabled={selectedStation === null}>
                    Save
                </Button>
            </Stack>
        </form>
    );
}

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
        <ThemeProvider theme={theme}>
            <CssBaseline />
            <main
                className={css`
                    padding: 16px;
                    color: white;
                    max-width: 600px;
                    margin: 0 auto;
                `}>
                <StationSelector />
                <VersionInfo />
            </main>
        </ThemeProvider>
    );
};

export default App;
