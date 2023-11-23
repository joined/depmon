import { css } from '@emotion/react';
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
    Box,
} from '@mui/material';
import React, { useState } from 'react';
import useSWRImmutable from 'swr/immutable';
import {
    SysInfoResponse,
    SysInfoSoftwareResponse,
    SysInfoMemoryResponse,
    SysInfoHardwareResponse,
    SysInfoTaskResponse,
} from 'src/api/Responses';
import { getRequestSender } from 'src/util/Ajax';
import { SYS_INFO_REFRESH_INTERVAL } from 'src/util/Constants';

const TASK_STATUS_TO_ICON: { [key: number]: React.ReactElement } = {
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
    <TableContainer component={Paper} css={bottomMarginStyle}>
        <Table>
            <TableBody>
                <TableRow key={'time'} css={lastTableRowStyle}>
                    <TableCell component="th" scope="row">
                        {KEY_TO_LABEL['time']}
                    </TableCell>
                    <TableCell align="right">
                        {props.data.time ? new Date(props.data.time).toISOString() : 'Not set'}
                    </TableCell>
                </TableRow>
                <TableRow key={'mdns_hostname'} css={lastTableRowStyle}>
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
    <TableContainer component={Paper} css={bottomMarginStyle}>
        <Table>
            <TableBody>
                {(
                    ['idf_version', 'project_name', 'compile_time', 'compile_date'] satisfies Array<
                        keyof SysInfoSoftwareResponse
                    >
                ).map((key) => (
                    <TableRow key={key} css={lastTableRowStyle}>
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
    <TableContainer component={Paper} css={bottomMarginStyle}>
        <Table>
            <TableBody>
                {(['free_heap', 'minimum_free_heap'] satisfies Array<keyof SysInfoMemoryResponse>).map((key) => (
                    <TableRow key={key} css={lastTableRowStyle}>
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
    <TableContainer component={Paper} css={bottomMarginStyle}>
        <Table>
            <TableBody>
                {/* TODO Add ESP version */}
                {(['mac_address'] satisfies Array<keyof SysInfoHardwareResponse>).map((key) => (
                    <TableRow key={key} css={lastTableRowStyle}>
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
    <TableContainer component={Paper} css={bottomMarginStyle}>
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
                    <TableRow key={`${task.name}-${task.core_id}`} css={lastTableRowStyle}>
                        <TableCell
                            css={css`
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

export const SystemInformationTab = () => {
    const [isAutoRefreshEnabled, setAutoRefreshEnabled] = useState(false);
    const { data, error, isLoading } = useSWRImmutable<SysInfoResponse>('/api/sysinfo', getRequestSender, {
        refreshInterval: isAutoRefreshEnabled ? SYS_INFO_REFRESH_INTERVAL : 0,
    });

    if (isLoading) {
        return <CircularProgress color="secondary" />;
    }

    if (error) {
        return <p>Error loading system information.</p>;
    }

    return (
        <Box sx={{ maxWidth: 800 }}>
            <Typography variant="h3" gutterBottom>
                System information
            </Typography>
            <FormGroup
                css={css`
                    margin-bottom: 16px;
                `}>
                <FormControlLabel
                    control={
                        <Checkbox
                            checked={isAutoRefreshEnabled}
                            onChange={(event) => {
                                setAutoRefreshEnabled(event.target.checked);
                            }}
                        />
                    }
                    label={`Auto-refresh every ${Math.floor(SYS_INFO_REFRESH_INTERVAL / 1000)}s`}
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
                        css={css`
                            max-width: calc(100vw - 64px);
                        `}>
                        <TaskTable data={data!.tasks} />
                    </div>
                </>
            ) : (
                <p>Task trace facility disabled.</p>
            )}
        </Box>
    );
};
