import { css } from '@emotion/css';
import AlarmOnIcon from '@mui/icons-material/AlarmOn';
import BlockIcon from '@mui/icons-material/Block';
import DeleteIcon from '@mui/icons-material/Delete';
import DirectionsRunIcon from '@mui/icons-material/DirectionsRun';
import PauseIcon from '@mui/icons-material/Pause';
import QuestionMarkIcon from '@mui/icons-material/QuestionMark';
import {
    CircularProgress,
    Typography,
    FormGroup,
    FormControlLabel,
    Checkbox,
    TableContainer,
    Paper,
    Table,
    TableBody,
    TableRow,
    TableCell,
    TableHead,
} from '@mui/material';
import { h, Fragment } from 'preact';
import { useState } from 'preact/hooks';
import { RoutableProps } from 'preact-router';
import useSWRImmutable from 'swr/immutable';
import {
    SysInfoResponse,
    SysInfoSoftwareResponse,
    SysInfoMemoryResponse,
    SysInfoHardwareResponse,
    SysInfoTaskResponse,
} from 'src/api/Responses';
import { getRequestSender } from 'src/util/Ajax';

const TASK_STATUS_TO_ICON: { [key: number]: h.JSX.Element } = {
    0: <DirectionsRunIcon />, // Running
    1: <AlarmOnIcon />, // Ready
    2: <BlockIcon />, // Blocked
    3: <PauseIcon />, // Suspended
    4: <DeleteIcon />, // Deleted
    5: <QuestionMarkIcon />, // Invalid
};

const KEY_TO_LABEL: { [key: string]: string } = {
    time: 'System time (UTC)',
    mdns_hostname: 'mDNS hostname',
    idf_version: 'IDF version',
    project_name: 'Project name',
    compile_time: 'Compile time',
    compile_date: 'Compile date',
    free_heap: 'Free heap',
    minimum_free_heap: 'Minimum free heap',
    mac_address: 'MAC address',
};

const bottomMarginStyle = css`
    margin-bottom: 16px;
`;
const lastTableRowStyle = css`
    &:last-child td,
    &:last-child th {
        border: 0;
    }
`;

const AppStateTable = (props: { data: SysInfoResponse['app_state'] }) => (
    <TableContainer component={Paper} className={bottomMarginStyle}>
        <Table>
            <TableBody>
                <TableRow key={'time'} className={lastTableRowStyle}>
                    <TableCell component="th" scope="row">
                        {KEY_TO_LABEL['time']}
                    </TableCell>
                    <TableCell align="right">
                        {props.data.time ? new Date(props.data.time).toISOString() : 'Not set'}
                    </TableCell>
                </TableRow>
                <TableRow key={'mdns_hostname'} className={lastTableRowStyle}>
                    <TableCell component="th" scope="row">
                        {KEY_TO_LABEL['mdns_hostname']}
                    </TableCell>
                    <TableCell align="right">{props.data.mdns_hostname.toLowerCase()}</TableCell>
                </TableRow>
            </TableBody>
        </Table>
    </TableContainer>
);

const SoftwareTable = (props: { data: SysInfoResponse['software'] }) => (
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
                        <TableCell align="right">{props.data[key]}</TableCell>
                    </TableRow>
                ))}
            </TableBody>
        </Table>
    </TableContainer>
);

const MemoryTable = (props: { data: SysInfoResponse['memory'] }) => (
    <TableContainer component={Paper} className={bottomMarginStyle}>
        <Table>
            <TableBody>
                {(['free_heap', 'minimum_free_heap'] satisfies Array<keyof SysInfoMemoryResponse>).map((key) => (
                    <TableRow key={key} className={lastTableRowStyle}>
                        <TableCell component="th" scope="row">
                            {KEY_TO_LABEL[key] || key}
                        </TableCell>
                        <TableCell align="right">{props.data[key]}</TableCell>
                    </TableRow>
                ))}
            </TableBody>
        </Table>
    </TableContainer>
);

const HardwareTable = (props: { data: SysInfoResponse['hardware'] }) => (
    // TODO Maybe use small variant of the table when there's little space?
    <TableContainer component={Paper} className={bottomMarginStyle}>
        <Table>
            <TableBody>
                {(['mac_address'] satisfies Array<keyof SysInfoHardwareResponse>).map((key) => (
                    <TableRow key={key} className={lastTableRowStyle}>
                        <TableCell component="th" scope="row">
                            {KEY_TO_LABEL[key] || key}
                        </TableCell>
                        <TableCell align="right">{props.data[key]}</TableCell>
                    </TableRow>
                ))}
            </TableBody>
        </Table>
    </TableContainer>
);

const TaskTable = (props: { data: NonNullable<SysInfoResponse['tasks']> }) => (
    <TableContainer component={Paper} className={bottomMarginStyle}>
        <Table>
            <TableHead>
                <TableRow>
                    <TableCell>Name</TableCell>
                    <TableCell align="right">Priority</TableCell>
                    <TableCell align="right">Stack high watermark</TableCell>
                    <TableCell align="right">State</TableCell>
                    {props.data[0].runtime !== null ? <TableCell align="right">Runtime</TableCell> : null}
                    {props.data[0].core_id !== null ? <TableCell align="right">Core ID</TableCell> : null}
                </TableRow>
            </TableHead>
            <TableBody>
                {props.data.map((task) => (
                    <TableRow key={task.name} className={lastTableRowStyle}>
                        <TableCell
                            className={css`
                                word-break: break-all;
                            `}>
                            {task.name}
                        </TableCell>
                        {(['priority', 'stack_high_water_mark'] satisfies Array<keyof SysInfoTaskResponse>).map(
                            (key) => (
                                <TableCell key={key} align="right">
                                    {task[key]}
                                </TableCell>
                            )
                        )}
                        <TableCell align="right">{TASK_STATUS_TO_ICON[task.state]}</TableCell>
                        {task.runtime !== null ? <TableCell align="right">{task.runtime}%</TableCell> : null}
                        {task.core_id !== null ? <TableCell align="right">{task.core_id}</TableCell> : null}
                    </TableRow>
                ))}
            </TableBody>
        </Table>
    </TableContainer>
);

// eslint-disable-next-line no-unused-vars
export const SystemInformationTab = (props: RoutableProps) => {
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
            <FormGroup
                className={css`
                    margin-bottom: 16px;
                `}>
                <FormControlLabel
                    control={
                        <Checkbox
                            checked={isAutoRefreshEnabled}
                            onInput={(event: Event) =>
                                setAutoRefreshEnabled((event.target as HTMLInputElement).checked)
                            }
                        />
                    }
                    label="Auto-refresh every 2s"
                />
            </FormGroup>
            <Typography variant="h4" gutterBottom>
                App state
            </Typography>
            <AppStateTable data={data!.app_state} />
            <Typography variant="h4" gutterBottom>
                Software
            </Typography>
            <SoftwareTable data={data!.software} />
            <Typography variant="h4" gutterBottom>
                Memory
            </Typography>
            <MemoryTable data={data!.memory} />
            <Typography variant="h4" gutterBottom>
                Hardware
            </Typography>
            <HardwareTable data={data!.hardware} />
            {data!.tasks ? (
                <>
                    <Typography variant="h4" gutterBottom>
                        FreeRTOS Tasks
                    </Typography>
                    {/* TODO This wrapper div is weird */}
                    <div
                        className={css`
                            max-width: calc(100vw - 64px);
                        `}>
                        <TaskTable data={data!.tasks} />
                    </div>
                </>
            ) : (
                <p>Task trace facility disabled.</p>
            )}
        </>
    );
};
