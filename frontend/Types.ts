export interface RouteConfig {
    path: string;
    name: string;
    drawerIcon: React.ReactElement;
}

export type LineProductType = 'suburban' | 'subway' | 'tram' | 'bus' | 'ferry' | 'express' | 'regional';

export type ParsedStationLine = [string, LineProductType];

export interface ParsedStation {
    id: string;
    name: string;
    lines: Array<ParsedStationLine>;
}
