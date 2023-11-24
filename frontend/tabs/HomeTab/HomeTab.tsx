import { css } from '@emotion/react';
import CloseIcon from '@mui/icons-material/Close';
import {
    AutocompleteRenderOptionState,
    CircularProgress,
    Typography,
    Stack,
    Autocomplete,
    TextField,
    Button,
    Snackbar,
    Alert,
    debounce,
    Grid,
    Box,
    Dialog,
    DialogTitle,
    IconButton,
    useMediaQuery,
    useTheme,
} from '@mui/material';
import match from 'autosuggest-highlight/match';
import parse from 'autosuggest-highlight/parse';
import axios from 'axios';
import { useState, useEffect, HTMLAttributes, useMemo } from 'react';
import { groupBy, uniqBy } from 'remeda';
import useSWRImmutable from 'swr/immutable';
import useSWRMutation from 'swr/mutation';
import { CurrentStationPostRequestSchema, LocationsQueryRequestQuerySchema } from '../../api/Requests';
import { CurrentStationGetResponse, LocationsQueryGetResponse, LocationsQueryResponseItem } from '../../api/Responses';
import { LineProductType, ParsedStation, ParsedStationLine } from '../../Types';
import { getRequestSender, postRequestSender } from '../../util/Ajax';

const getLocationsQueryURL = (query: string) => {
    const url = new URL('https://v6.bvg.transport.rest/locations');

    const queryParams: LocationsQueryRequestQuerySchema = {
        query,
        fuzzy: true,
        results: 10,
        stops: true,
        addresses: false,
        poi: false,
        linesOfStops: true,
        language: 'en',
    };

    Object.keys(queryParams).forEach((key) => {
        url.searchParams.append(key, queryParams[key].toString());
    });

    return url.href;
};

const parseStationItem = ({ id, name, lines }: LocationsQueryResponseItem): ParsedStation => {
    // Seems like some stations don't have an ID
    const filteredLines = lines.filter((line) => line.id);
    // Seems like some stations are reported multiple times, the first one looks like the right one
    const uniqueLines = uniqBy(filteredLines, (line) => line.id);

    const lineNamesWithProducts = uniqueLines.map(
        (line) => [line.name, line.id?.startsWith('r') ? 'regional' : line.product] as [string, LineProductType]
    );

    return {
        id,
        name,
        lines: lineNamesWithProducts,
    };
};

interface LineIconProps {
    name: string;
    type: LineProductType;
    fontSize?: 'small' | 'medium';
}

function LineIcon({ name, type, fontSize = 'medium' }: LineIconProps) {
    const fillColor = useMemo(() => {
        switch (type) {
            case 'bus':
                return 'var(--purple)';
            case 'tram':
                return 'var(--tram-red)';
            case 'suburban':
                return 'var(--green)';
            case 'subway':
                return 'var(--blue)';
            case 'ferry':
                // TODO Check
                return 'var(--blue)';
            case 'express':
                // TODO Check
                return 'var(--db-red)';
            case 'regional':
                return 'var(--db-red)';
        }
    }, [type]);

    return (
        <svg
            viewBox="0 0 130 50"
            css={css`
                border-radius: 5px;
            `}
            xmlns="http://www.w3.org/2000/svg">
            <rect
                css={css`
                    fill: ${fillColor};
                    width: 130px;
                    height: 50px;
                `}
            />
            <text
                x="50%"
                y="50%"
                css={css`
                    fill: #fff;
                    dominant-baseline: central;
                    text-anchor: middle;
                    font-size: ${fontSize === 'small' ? '41px' : '50px'};
                    font-family: Roboto;
                    font-weight: 700;
                `}>
                {name}
            </text>
        </svg>
    );
}

export interface StationChangeDialogProps {
    currentStationId?: string;
    open: boolean;
    onClose: () => void;
}

function StationChangeDialog(props: StationChangeDialogProps) {
    const { onClose, open } = props;
    const [stations, setStations] = useState<Array<ParsedStation>>([]);
    const [selectedStation, setSelectedStation] = useState<ParsedStation | null>(null);
    const [inputValue, setInputValue] = useState('');
    const [isSnackbarOpen, setSnackbarOpen] = useState(false);

    const { trigger: saveSelectedStation, isMutating } = useSWRMutation(
        '/api/currentstation',
        postRequestSender<CurrentStationPostRequestSchema>,
        {
            onSuccess: () => {
                props.onClose();
            },
            onError: () => {
                setSnackbarOpen(true);
            },
        }
    );

    const fetchCurrentStationDebounced = useMemo(
        () =>
            debounce((query: string, callback: (response: Array<ParsedStation>) => void) => {
                axios.get<LocationsQueryGetResponse>(getLocationsQueryURL(query)).then((response) => {
                    callback(response.data.map(parseStationItem) ?? []);
                });
            }, 600),
        []
    );

    useEffect(() => {
        if (inputValue) {
            fetchCurrentStationDebounced(inputValue, setStations);
        }
    }, [fetchCurrentStationDebounced, inputValue, selectedStation]);

    const handleClose = () => {
        onClose();
    };

    const renderOption = (
        props: HTMLAttributes<HTMLLIElement>,
        { name, lines }: ParsedStation,
        state: AutocompleteRenderOptionState
    ) => {
        const matches = match(name, state.inputValue, { insideWords: true });
        const parts = parse(name, matches);

        return (
            <li {...props}>
                <Grid container alignItems="center">
                    <Grid item css={css`width: 'calc(100% - 44px)', word-wrap: 'break-word'`}>
                        {parts.map((part, index) => (
                            <Box
                                key={index}
                                component="span"
                                css={css`
                                    font-weight: ${part.highlight ? 'bold' : 'regular'};
                                `}>
                                {part.text}
                            </Box>
                        ))}
                    </Grid>
                    <Grid item container columnGap={0.5} rowGap={0.5}>
                        {lines.map(([name, product]) => (
                            <Grid
                                item
                                key={name}
                                css={css`
                                    width: 40px;
                                    height: 15px;
                                `}>
                                <LineIcon name={name} type={product} />
                            </Grid>
                        ))}
                    </Grid>
                </Grid>
            </li>
        );
    };

    const isSubmissionDisabled =
        selectedStation === null || selectedStation.id === props.currentStationId || isMutating;

    const theme = useTheme();
    const fullScreen = useMediaQuery(theme.breakpoints.down('sm'));

    return (
        <Dialog fullScreen={fullScreen} onClose={handleClose} open={open} PaperProps={{ sx: { overflowY: 'visible' } }}>
            <DialogTitle>Select new station</DialogTitle>
            <IconButton
                aria-label="close"
                onClick={handleClose}
                sx={{
                    position: 'absolute',
                    right: 8,
                    top: 8,
                    color: (theme) => theme.palette.grey[500],
                }}>
                <CloseIcon />
            </IconButton>
            <Stack
                component="form"
                onSubmit={(event) => {
                    event.preventDefault();
                    saveSelectedStation(selectedStation!);
                }}
                direction={{ xs: 'column', sm: 'row' }}
                padding={2}
                spacing={2}>
                <Autocomplete<ParsedStation>
                    disablePortal
                    autoComplete
                    includeInputInList
                    filterSelectedOptions
                    id="station-selector"
                    noOptionsText="No stations."
                    options={stations}
                    filterOptions={(x) => x}
                    inputValue={inputValue}
                    onInputChange={(event, newInputValue) => {
                        setInputValue(newInputValue);
                    }}
                    value={selectedStation}
                    onChange={(event, newStation) => {
                        setStations(newStation ? [newStation, ...stations] : stations);
                        setSelectedStation(newStation);
                    }}
                    sx={{ width: { sm: 450 } }}
                    renderInput={(params) => <TextField {...params} label="Station" placeholder="Search station" />}
                    getOptionLabel={(option) => option.name}
                    renderOption={renderOption}
                />
                <Button type="submit" variant="contained" disabled={isSubmissionDisabled}>
                    Save
                </Button>
            </Stack>
            <Snackbar open={isSnackbarOpen} autoHideDuration={3000} onClose={() => setSnackbarOpen(false)}>
                <Alert severity="error" variant="filled">
                    Error saving station
                </Alert>
            </Snackbar>
        </Dialog>
    );
}

interface ServicesRowProps {
    product: LineProductType;
    lines: Array<ParsedStationLine>;
}

function ServicesProductRow({ product, lines }: ServicesRowProps) {
    /* TODO Add checkboxes, make them do something */
    /* TODO Reset checkbox state on station selection */
    /* TODO If there is just one product, do not show the checkbox */

    return (
        <Stack direction={{ xs: 'column', md: 'row' }} gap={1}>
            <Typography variant="body1" sx={{ minWidth: 100 }}>
                {product.charAt(0).toUpperCase() + product.slice(1)}
            </Typography>
            <Grid container key={product} columnGap={1} rowGap={1} marginBottom={1}>
                {lines.map(([name, product]) => (
                    // TODO Figure out what's the deal with the SVG size
                    <Grid
                        item
                        key={name}
                        css={css`
                            width: 60px;
                            height: 23px;
                        `}>
                        <LineIcon name={name} type={product} fontSize="small" />
                    </Grid>
                ))}
            </Grid>
        </Stack>
    );
}

interface ServicesSectionProps {
    lines: Array<ParsedStationLine>;
}

function ServicesSection({ lines }: ServicesSectionProps) {
    const linesGroupedByProduct = groupBy(lines, (line) => line[1]);

    return (
        <>
            <Typography variant="h4" gutterBottom>
                Services
            </Typography>
            <Stack direction="column" gap={1}>
                {Object.entries(linesGroupedByProduct).map(([product, lines]) => (
                    <ServicesProductRow key={product} product={product as LineProductType} lines={lines} />
                ))}
            </Stack>
        </>
    );
}

export function HomeTab() {
    const [isStationChangeDialogOpen, setStationChangeDialogOpen] = useState(false);
    const {
        data: currentStationResponse,
        error,
        isLoading,
    } = useSWRImmutable<CurrentStationGetResponse>('/api/currentstation', getRequestSender);

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
                <>
                    <Typography variant="body1" gutterBottom>
                        No station selected. Please select a station to show departures from.
                    </Typography>
                    <Button sx={{ mt: 1 }} variant="contained" onClick={() => setStationChangeDialogOpen(true)}>
                        Select station
                    </Button>
                </>
            ) : (
                <>
                    <Stack
                        direction={{ xs: 'column', sm: 'row' }}
                        justifyContent={{ sm: 'space-between' }}
                        alignItems={{ sm: 'center' }}
                        gap={{ xs: 1, sm: 2 }}
                        marginBottom={2}>
                        <Stack direction={{ xs: 'column', md: 'row' }} gap={1}>
                            <Box>Currently selected station:</Box>
                            <Box sx={{ fontWeight: 'bold' }} display="inline">
                                {currentStationResponse ? `${currentStationResponse.name}` : 'None'}
                            </Box>
                        </Stack>
                        <Button
                            sx={{ width: { xs: 100, sm: 'auto' } }}
                            variant="contained"
                            onClick={() => setStationChangeDialogOpen(true)}>
                            Change
                        </Button>
                    </Stack>
                    <Box>
                        <ServicesSection lines={currentStationResponse.lines} />
                    </Box>
                    {/* TODO Add ability to configure `duration` parameter of transport.rest request */}
                </>
            )}
            <StationChangeDialog
                currentStationId={currentStationResponse?.id}
                open={isStationChangeDialogOpen}
                onClose={() => setStationChangeDialogOpen(false)}
            />
        </Box>
    );
}
