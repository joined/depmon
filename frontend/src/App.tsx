import { h } from 'preact';
import BlockIcon from '@mui/icons-material/Block';
import DeleteIcon from '@mui/icons-material/Delete';
import PauseIcon from '@mui/icons-material/Pause';
import QuestionMarkIcon from '@mui/icons-material/QuestionMark';
import DirectionsRunIcon from '@mui/icons-material/DirectionsRun';
import AlarmOnIcon from '@mui/icons-material/AlarmOn';
import { css } from '@emotion/css';
import { Station, stations } from './stations';
import {
    Alert,
    Autocomplete,
    AutocompleteRenderOptionState,
    Button,
    Checkbox,
    CircularProgress,
    CssBaseline,
    FormControlLabel,
    FormGroup,
    Paper,
    Snackbar,
    Stack,
    Table,
    TableBody,
    TableCell,
    TableContainer,
    TableHead,
    TableRow,
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

const TASK_STATUS_TO_ICON: { [key: number]: h.JSX.Element } = {
    0: <DirectionsRunIcon />, // Running
    1: <AlarmOnIcon />, // Ready
    2: <BlockIcon />, // Blocked
    3: <PauseIcon />, // Suspended
    4: <DeleteIcon />, // Deleted
    5: <QuestionMarkIcon />, // Invalid
};

const KEY_TO_LABEL: { [key: string]: string } = {
    idf_version: 'IDF version',
    project_name: 'Project name',
    compile_time: 'Compile time',
    compile_date: 'Compile date',
    free_heap: 'Free heap',
    minimum_free_heap: 'Minimum free heap',
    mac_address: 'MAC address',
};

interface SysInfoSoftwareResponse {
    idf_version: string;
    project_name: string;
    compile_time: string;
    compile_date: string;
}

interface SysInfoHardwareResponse {
    mac_address: string;
}

interface SysInfoMemoryResponse {
    free_heap: number;
    minimum_free_heap: number;
}

interface SysInfoTaskResponse {
    name: string;
    priority: number;
    state: number;
    stack_high_water_mark: number;
    runtime: number | null;
    core_id: number | null;
}

interface SysInfoResponse {
    software: SysInfoSoftwareResponse;
    hardware: SysInfoHardwareResponse;
    memory: SysInfoMemoryResponse;
    tasks: SysInfoTaskResponse[] | null;
}

const bottomMarginStyle = css`
    margin-bottom: 16px;
`;
const lastTableRowStyle = css`
    &:last-child td,
    &:last-child th {
        border: 0;
    }
`;

const SysInfo = () => {
    const [isAutoRefreshEnabled, setAutoRefreshEnabled] = useState(false);
    const { data, error, isLoading } = useSWRImmutable<SysInfoResponse>('/api/sysinfo', getRequestSender, {
        refreshInterval: isAutoRefreshEnabled ? 2000 : 0,
    });

    if (isLoading) {
        return <CircularProgress color="secondary" />;
    }

    if (error) {
        return <p>Error loading system information.</p>;
    }

    return (
        <>
            <Typography variant="h3" gutterBottom>
                System information
            </Typography>
            <FormGroup className={css`margin-bottom: 16px`}>
                <FormControlLabel
                    control={
                        <Checkbox
                            checked={isAutoRefreshEnabled}
                            onInput={(event: React.ChangeEvent<HTMLInputElement>) =>
                                setAutoRefreshEnabled((event.target as HTMLInputElement).checked)
                            }
                        />
                    }
                    label="Auto-refresh every 2s"
                />
            </FormGroup>
            <Typography variant="h4" gutterBottom>
                Software
            </Typography>
            <TableContainer component={Paper} className={bottomMarginStyle}>
                <Table>
                    <TableBody>
                        {(
                            ['idf_version', 'project_name', 'compile_time', 'compile_date'] satisfies Array<
                                keyof SysInfoSoftwareResponse
                            >
                        ).map((key) => (
                            <TableRow key={key} className={lastTableRowStyle}>
                                <TableCell component="th" scope="row">
                                    {KEY_TO_LABEL[key] || key}
                                </TableCell>
                                <TableCell align="right">{data!.software[key]}</TableCell>
                            </TableRow>
                        ))}
                    </TableBody>
                </Table>
            </TableContainer>
            <Typography variant="h4" gutterBottom>
                Memory
            </Typography>
            <TableContainer component={Paper} className={bottomMarginStyle}>
                <Table>
                    <TableBody>
                        {(['free_heap', 'minimum_free_heap'] satisfies Array<keyof SysInfoMemoryResponse>).map(
                            (key) => (
                                <TableRow key={key} className={lastTableRowStyle}>
                                    <TableCell component="th" scope="row">
                                        {KEY_TO_LABEL[key] || key}
                                    </TableCell>
                                    <TableCell align="right">{data!.memory[key]}</TableCell>
                                </TableRow>
                            )
                        )}
                    </TableBody>
                </Table>
            </TableContainer>
            <Typography variant="h4" gutterBottom>
                Hardware
            </Typography>
            <TableContainer component={Paper} className={bottomMarginStyle}>
                <Table>
                    <TableBody>
                        {(['mac_address'] satisfies Array<keyof SysInfoHardwareResponse>).map((key) => (
                            <TableRow key={key} className={lastTableRowStyle}>
                                <TableCell component="th" scope="row">
                                    {KEY_TO_LABEL[key] || key}
                                </TableCell>
                                <TableCell align="right">{data!.hardware[key]}</TableCell>
                            </TableRow>
                        ))}
                    </TableBody>
                </Table>
            </TableContainer>
            {data!.tasks ? (
                <>
                    <Typography variant="h4" gutterBottom>
                        FreeRTOS Tasks
                    </Typography>
                    <TableContainer component={Paper} className={bottomMarginStyle}>
                        <Table size="medium">
                            <TableHead>
                                <TableRow>
                                    <TableCell>Name</TableCell>
                                    <TableCell align="right">Priority</TableCell>
                                    <TableCell align="right">Stack high watermark</TableCell>
                                    <TableCell align="right">State</TableCell>
                                    {data!.tasks[0].runtime !== null ? (
                                        <TableCell align="right">Runtime</TableCell>
                                    ) : null}
                                    {data!.tasks[0].core_id !== null ? (
                                        <TableCell align="right">Core ID</TableCell>
                                    ) : null}
                                </TableRow>
                            </TableHead>
                            <TableBody>
                                {data!.tasks.map((task) => (
                                    <TableRow key={task.name} className={lastTableRowStyle}>
                                        {(
                                            ['name', 'priority', 'stack_high_water_mark'] satisfies Array<
                                                keyof SysInfoTaskResponse
                                            >
                                        ).map((key) => (
                                            <TableCell
                                                align="right"
                                                className={
                                                    key === 'name'
                                                        ? css`
                                                              word-break: break-all;
                                                          `
                                                        : undefined
                                                }>
                                                {task[key]}
                                            </TableCell>
                                        ))}
                                        <TableCell align="right">{TASK_STATUS_TO_ICON[task.state]}</TableCell>
                                        {task.runtime !== null ? (
                                            <TableCell align="right">{task.runtime}%</TableCell>
                                        ) : null}
                                        {task.core_id !== null ? (
                                            <TableCell align="right">{task.core_id}</TableCell>
                                        ) : null}
                                    </TableRow>
                                ))}
                            </TableBody>
                        </Table>
                    </TableContainer>
                </>
            ) : (
                <p>Task trace facility disabled.</p>
            )}
        </>
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
                <SysInfo />
            </main>
        </ThemeProvider>
    );
};

export default App;
