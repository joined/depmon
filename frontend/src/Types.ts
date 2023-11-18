import { h } from "preact";
import { RoutableProps } from "preact-router";

export interface RouteConfig {
    path: string;
    name: string;
    drawerIcon: h.JSX.Element;
    tabComponent: (props: RoutableProps) => h.JSX.Element;
}