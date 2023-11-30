import { css } from "@emotion/react";
import { useMemo } from "react";
import { LineProductType } from "frontend/Types";

interface LineIconProps {
    name: string;
    type: LineProductType;
    fontSize?: 'small' | 'medium';
    disabled?: boolean;
}

export default function LineIcon({ name, type, disabled, fontSize = 'medium' }: LineIconProps) {
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
                    opacity: ${disabled ? 0.3 : 1};
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
                    opacity: ${disabled ? 0.3 : 1};
                `}>
                {name}
            </text>
        </svg>
    );
}