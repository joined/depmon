import HomeIcon from '@mui/icons-material/Home';
import InfoIcon from '@mui/icons-material/Info';
import MenuIcon from '@mui/icons-material/Menu';
import {
    AppBar,
    Box,
    CssBaseline,
    Drawer,
    IconButton,
    List,
    ListItem,
    ListItemButton,
    ListItemIcon,
    ListItemText,
    ThemeProvider,
    Toolbar,
    Typography,
    createTheme,
} from '@mui/material';
import { h } from 'preact';
import { useState } from 'preact/compat';

import { route as navigate, Router } from 'preact-router';
import { HomeTab } from './components/HomeTab/HomeTab';
import { SystemInformationTab } from './components/SystemInformationTab/SystemInformationTab';
import { RouteConfig } from './Types';
import { DRAWER_WIDTH } from './util/Constants';

const theme = createTheme({
    palette: {
        mode: 'dark',
    },
});

const ROUTES: RouteConfig[] = [
    {
        path: '/',
        name: 'Home',
        drawerIcon: <HomeIcon />,
        tabComponent: HomeTab,
    },
    {
        path: '/sysinfo',
        name: 'System information',
        drawerIcon: <InfoIcon />,
        tabComponent: SystemInformationTab,
    },
];

const NavigationDrawerContent = () => (
    <List>
        {ROUTES.map((route) => (
            <ListItem key={route.path} disablePadding>
                <ListItemButton
                    onClick={() => {
                        navigate(route.path);
                    }}>
                    <ListItemIcon>{route.drawerIcon}</ListItemIcon>
                    <ListItemText primary={route.name} />
                </ListItemButton>
            </ListItem>
        ))}
    </List>
);

const App = () => {
    const [mobileOpen, setMobileOpen] = useState(false);

    return (
        <ThemeProvider theme={theme}>
            <Box sx={{ display: 'flex' }}>
                <CssBaseline />
                <AppBar
                    position="fixed"
                    sx={{
                        width: { sm: `calc(100% - ${DRAWER_WIDTH}px)` },
                        ml: { sm: `${DRAWER_WIDTH}px` },
                    }}>
                    <Toolbar>
                        <IconButton
                            color="inherit"
                            edge="start"
                            onClick={() => setMobileOpen(true)}
                            sx={{ mr: 2, display: { sm: 'none' } }}>
                            <MenuIcon />
                        </IconButton>
                        {/* TODO On mobile show the tab name? */}
                        <Typography variant="h6" noWrap component="div">
                            DepMon
                        </Typography>
                    </Toolbar>
                </AppBar>
                <Box
                    component="nav"
                    sx={{ width: { sm: DRAWER_WIDTH }, flexShrink: { sm: 0 } }}
                    aria-label="mailbox folders">
                    <Drawer
                        variant="temporary"
                        open={mobileOpen}
                        onClose={() => {
                            setMobileOpen(false);
                        }}
                        ModalProps={{
                            keepMounted: true,
                        }}
                        sx={{
                            display: { xs: 'block', sm: 'none' },
                            '& .MuiDrawer-paper': { boxSizing: 'border-box', width: DRAWER_WIDTH },
                        }}>
                        <NavigationDrawerContent />
                    </Drawer>
                    <Drawer
                        variant="permanent"
                        sx={{
                            display: { xs: 'none', sm: 'block' },
                            '& .MuiDrawer-paper': { boxSizing: 'border-box', width: DRAWER_WIDTH },
                        }}
                        open>
                        <NavigationDrawerContent />
                    </Drawer>
                </Box>
                <Box component="main" sx={{ flexGrow: 1, p: 3, width: { sm: `calc(100% - ${DRAWER_WIDTH}px)` } }}>
                    <Toolbar />
                    <Router>
                        {/* TODO Make these work in the ESP backend */}
                        {ROUTES.map((route) => (
                            <route.tabComponent key={route.path} path={route.path} />
                        ))}
                    </Router>
                </Box>
            </Box>
        </ThemeProvider>
    );
};

export default App;
