import { h } from 'preact';
import { useState } from 'preact/hooks';
import { css } from '@emotion/css';
import Button from '@mui/material/Button';
import { useFetch } from 'usehooks-ts';
import {
    AppBar,
    Box,
    CircularProgress,
    CssBaseline,
    Tab,
    Tabs,
    ThemeProvider,
    Typography,
    createTheme,
} from '@mui/material';

const darkTheme = createTheme({
    palette: {
        mode: 'dark',
    },
});

interface TabPanelProps {
    children?: React.ReactNode;
    index: number;
    value: number;
}

function TabPanel(props: TabPanelProps) {
    const { children, value, index, ...other } = props;

    return (
        <div role="tabpanel" hidden={value !== index} {...other}>
            {value === index && (
                <Box sx={{ p: 3 }}>
                    <Typography>{children}</Typography>
                </Box>
            )}
        </div>
    );
}

function ScrollableTabs() {
    const [value, setValue] = useState(0);

    const handleChange = (event: any, newValue: number) => {
        setValue(newValue);
    };

    return (
        <Box sx={{ bgcolor: 'background.paper' }}>
            <AppBar position="static">
                <Tabs value={value} onChange={handleChange} variant="scrollable" scrollButtons="auto">
                    <Tab label="Main" />
                    <Tab label="Info" />
                    <Tab label="Item Three" />
                    <Tab label="Item Four" />
                </Tabs>
            </AppBar>
            <TabPanel value={value} index={0}>
                Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla convallis egestas rhoncus. Donec
                facilisis fermentum sem, ac viverra ante luctus vel. Donec vel mauris quam. Lorem ipsum dolor sit amet,
                consectetur adipiscing elit. Nulla convallis egestas rhoncus. Donec facilisis fermentum sem, ac viverra
                ante luctus vel. Donec vel mauris quam. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla
                convallis egestas rhoncus. Donec facilisis fermentum sem, ac viverra ante luctus vel. Donec vel mauris
                quam.
            </TabPanel>
            <TabPanel value={value} index={1}>
                <VersionInfo />
            </TabPanel>
            <TabPanel value={value} index={2}>
            Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla convallis egestas rhoncus. Donec
                facilisis fermentum sem, ac viverra ante luctus vel. Donec vel mauris quam. Lorem ipsum dolor sit amet,
                consectetur adipiscing elit. Nulla convallis egestas rhoncus. Donec facilisis fermentum sem, ac viverra
                ante luctus vel. Donec vel mauris quam. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla
                convallis egestas rhoncus. Donec facilisis fermentum sem, ac viverra ante luctus vel. Donec vel mauris
                quam.
            </TabPanel>
            <TabPanel value={value} index={3}>
            Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla convallis egestas rhoncus. Donec
                facilisis fermentum sem, ac viverra ante luctus vel. Donec vel mauris quam. Lorem ipsum dolor sit amet,
                consectetur adipiscing elit. Nulla convallis egestas rhoncus. Donec facilisis fermentum sem, ac viverra
                ante luctus vel. Donec vel mauris quam. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla
                convallis egestas rhoncus. Donec facilisis fermentum sem, ac viverra ante luctus vel. Donec vel mauris
                quam.
            </TabPanel>
        </Box>
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
                <ScrollableTabs />
            </main>
        </ThemeProvider>
    );
};

export default App;
