import { css } from '@emotion/css';
import {
    createFilterOptions,
    AutocompleteRenderOptionState,
    CircularProgress,
    Typography,
    Stack,
    Autocomplete,
    TextField,
    Button,
    Snackbar,
    Alert,
} from '@mui/material';
import match from 'autosuggest-highlight/match';
import parse from 'autosuggest-highlight/parse';
import { useState, useMemo, useEffect, HTMLAttributes } from 'react';
import useSWRImmutable from 'swr/immutable';
import useSWRMutation from 'swr/mutation';
import { CurrentStationPostRequest } from 'src/api/Requests';
import { CurrentStationGetResponse } from 'src/api/Responses';
import { Station, stations } from 'src/stations';
import { getRequestSender, postRequestSender } from 'src/util/Ajax';

export function HomeTab() {
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
        setSelectedStation((previousSelectedStation) =>
            data && previousSelectedStation?.id !== data.id
                ? stations.find((station) => station.id === data.id) || null
                : previousSelectedStation
        );
    }, [data]);

    const renderOption = (
        props: HTMLAttributes<HTMLLIElement>,
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
            <Typography variant="h3" gutterBottom>
                Station selector
            </Typography>
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
                        onChange={(event, newStation) => {
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
