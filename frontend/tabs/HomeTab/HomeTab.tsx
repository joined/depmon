import { CircularProgress, Typography, Stack, Button, Snackbar, Alert, Box } from '@mui/material';
import { useState } from 'react';

import useSWR from 'swr';
import useSWRMutation from 'swr/mutation';
import { CurrentStationPostRequestSchema } from '../../api/Requests';
import { CurrentStationGetResponse } from '../../api/Responses';
import { getRequestSender, postRequestSender } from '../../util/Ajax';
import ServicesSection from './ServicesSection';
import StationChangeDialog from './StationChangeDialog';
import { useSnackbarState } from './useSnackbarState';

interface InitialConfigurationProps {
    onButtonClick: () => void;
    disabled: boolean;
}

function InitialConfiguration({ onButtonClick, disabled }: InitialConfigurationProps) {
    return (
        <Box>
            <Typography variant="body1" gutterBottom>
                No station selected. Please select a station to show departures from.
            </Typography>
            <Button disabled={disabled} sx={{ mt: 1 }} variant="contained" onClick={onButtonClick}>
                Select station
            </Button>
        </Box>
    );
}

export function HomeTab() {
    const [isStationChangeDialogOpen, setStationChangeDialogOpen] = useState(false);
    const {
        data: currentStationResponse,
        error,
        isLoading,
        isValidating,
    } = useSWR<CurrentStationGetResponse>('/api/currentstation', getRequestSender);
    const { trigger, isMutating } = useSWRMutation(
        '/api/currentstation',
        postRequestSender<CurrentStationPostRequestSchema>,
        { revalidate: false }
    );

    const { state: snackbarState, openWithMessage: openSnackbarWithMessage, close: closeSnackbar } = useSnackbarState();

    if (isLoading) {
        return <CircularProgress color="secondary" />;
    }

    if (error) {
        return <p>Error loading station information.</p>;
    }

    return (
        <Box maxWidth={800}>
            <Typography variant="h3" gutterBottom>
                Departures panel configuration
            </Typography>
            {!currentStationResponse ? (
                <InitialConfiguration disabled={isValidating} onButtonClick={() => setStationChangeDialogOpen(true)} />
            ) : (
                <Box>
                    <Stack
                        direction={{ xs: 'column', sm: 'row' }}
                        justifyContent={{ sm: 'space-between' }}
                        alignItems={{ sm: 'center' }}
                        gap={{ xs: 1, sm: 2 }}
                        marginBottom={2}>
                        <Stack direction={{ xs: 'column', md: 'row' }} gap={1}>
                            <Box>Currently selected station:</Box>
                            <Box sx={{ fontWeight: 'bold' }} display="inline">
                                {currentStationResponse.name}
                            </Box>
                        </Stack>
                        <Button
                            disabled={isValidating || isMutating}
                            sx={{ width: { xs: 100, sm: 'auto' } }}
                            variant="contained"
                            onClick={() => setStationChangeDialogOpen(true)}>
                            Change
                        </Button>
                    </Stack>
                    <ServicesSection
                        currentStation={currentStationResponse}
                        saveNewCurrentStation={(newCurrentStation) => {
                            trigger(newCurrentStation, {
                                onSuccess: () => {
                                    openSnackbarWithMessage('Saved', 'success');
                                },
                                onError: () => {
                                    openSnackbarWithMessage('Error, please try again', 'error');
                                },
                                optimisticData: newCurrentStation,
                            });
                        }}
                        disableToggles={isMutating || isValidating}
                    />
                </Box>
            )}
            <StationChangeDialog
                currentStationId={currentStationResponse?.id ?? null}
                open={isStationChangeDialogOpen}
                saveNewCurrentStation={(newCurrentStation, onSuccess) => {
                    trigger(newCurrentStation, {
                        onSuccess: () => {
                            openSnackbarWithMessage('Station changed successfully', 'success');
                            onSuccess();
                            setStationChangeDialogOpen(false);
                        },
                        onError: () => {
                            openSnackbarWithMessage('Error, please try again', 'error');
                        },
                        optimisticData: newCurrentStation,
                    });
                }}
                onClose={() => setStationChangeDialogOpen(false)}
                isMutating={isMutating}
            />
            <Snackbar open={snackbarState.open} autoHideDuration={3000} onClose={closeSnackbar}>
                <Alert severity={snackbarState.severity} variant="filled">
                    {snackbarState.message}
                </Alert>
            </Snackbar>
        </Box>
    );
}
