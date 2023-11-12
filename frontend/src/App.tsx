import { h } from 'preact';
import { css } from '@emotion/css';
import { Station, stations } from './stations';
import {
    Alert,
    Autocomplete,
    AutocompleteRenderOptionState,
    Button,
    CircularProgress,
    CssBaseline,
    Snackbar,
    Stack,
    TextField,
    ThemeProvider,
    Typography,
    createFilterOptions,
    createTheme,
} from '@mui/material';
import parse from 'autosuggest-highlight/parse';
import match from 'autosuggest-highlight/match';
import { useEffect, useMemo, useState, Fragment } from 'preact/compat';
import useSWRImmutable from 'swr/immutable';
import useSWRMutation from 'swr/mutation';
import axios from 'axios';

const theme = createTheme({
    palette: {
        mode: 'dark',
    },
});

interface CurrentStationGetResponse {
    id: string;
}

type CurrentStationPostRequest = CurrentStationGetResponse;

const getRequestSender = (url: string) => axios.get(url).then((res) => res.data);
const postRequestSender = <TRequestBody,>(url: string, { arg }: { arg: TRequestBody }) => axios.post(url, { ...arg });

export function StationSelector() {
    const [selectedStation, setSelectedStation] = useState<Station | null>(null);
    const [inputValue, setInputValue] = useState('');
    const [isSnackbarOpen, setSnackbarOpen] = useState(false);
    const [lastPostRequestOutcome, setLastPostRequestOutcome] = useState<'success' | 'error' | null>(null);

    const filterOptions = useMemo(
        () =>
            createFilterOptions<Station>({
                ignoreCase: true,
                limit: 10,
            }),
        []
    );

    const { data, error, isLoading } = useSWRImmutable<CurrentStationGetResponse>(
        '/api/currentstation',
        getRequestSender
    );
    const { trigger, isMutating } = useSWRMutation(
        '/api/currentstation',
        postRequestSender<CurrentStationPostRequest>,
        {
            optimisticData: { id: selectedStation?.id },
            revalidate: false,
            onSuccess: () => {
                setLastPostRequestOutcome('success');
                setSnackbarOpen(true);
            },
            onError: () => {
                setLastPostRequestOutcome('error');
                setSnackbarOpen(true);
            },
        }
    );

    useEffect(() => {
        // On first page load and on error, update the selected station to the current station.
        if (data && selectedStation?.id !== data.id) {
            setSelectedStation(stations.find((station) => station.id === data.id) || null);
        }
    }, [data]);

    const renderOption = (
        props: React.HTMLAttributes<HTMLLIElement>,
        option: Station,
        state: AutocompleteRenderOptionState
    ) => {
        const matches = match(option.name, state.inputValue, { insideWords: true });
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
    };

    if (isLoading) {
        return <CircularProgress color="secondary" />;
    }

    if (error) {
        return <p>Error loading station information.</p>;
    }

    return (
        <>
            <form
                className={css`
                    margin-bottom: 16px;
                `}
                onSubmit={(event) => {
                    event.preventDefault();
                    trigger({ id: selectedStation!.id });
                }}>
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
                        renderOption={renderOption}
                    />
                    <Button
                        type="submit"
                        variant="contained"
                        disabled={selectedStation === null || selectedStation.id === data?.id || isMutating}>
                        Save
                    </Button>
                </Stack>
            </form>
            <Snackbar open={isSnackbarOpen} autoHideDuration={2000} onClose={() => setSnackbarOpen(false)}>
                <Alert severity={lastPostRequestOutcome!} variant="filled">
                    {lastPostRequestOutcome === 'success' ? 'Saved!' : 'Error saving station'}
                </Alert>
            </Snackbar>
        </>
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
    const { data, error, isLoading } = useSWRImmutable<VersionResponse>('/api/version', getRequestSender);

    if (isLoading) {
        return <CircularProgress color="secondary" />;
    }

    if (error) {
        return <p>Error loading version information.</p>;
    }

    return (
        <ul>
            <li>
                <strong>Version:</strong> {data!.version}
            </li>
            <li>
                <strong>ESP-IDF version:</strong> {data!.idf_version}
            </li>
            <li>
                <strong>Project name:</strong> {data!.project_name}
            </li>
            <li>
                <strong>Compile time:</strong> {data!.compile_time}
            </li>
            <li>
                <strong>Compile date:</strong> {data!.compile_date}
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
                <Typography variant="h1" gutterBottom>
                    DepMon
                </Typography>
                <StationSelector />
                <Typography variant="h3" gutterBottom>
                    Version information
                </Typography>
                <VersionInfo />
            </main>
        </ThemeProvider>
    );
};

export default App;
