import React from 'react';
import { createRoot } from 'react-dom/client';
import { RouterProvider, createBrowserRouter } from 'react-router-dom';
import { Root } from './Root';
import { HomeTab } from './tabs/HomeTab/HomeTab';
import { SystemInformationTab } from './tabs/SystemInformationTab/SystemInformationTab';

const router = createBrowserRouter([
    {
        path: '/',
        element: <Root />,
        children: [
            {
                index: true,
                element: <HomeTab />,
            },
            {
                path: '/sysinfo',
                element: <SystemInformationTab />,
            },
        ],
    },
]);

const root = createRoot(document.getElementById('root')!);
root.render(
    <React.StrictMode>
        <RouterProvider router={router} />
    </React.StrictMode>
);
