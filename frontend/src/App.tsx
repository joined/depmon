import { h } from 'preact';
import { useState } from 'preact/hooks';
import { css } from '@emotion/css';
import Button from '@mui/material/Button';

const App = () => {
    const [count, setCount] = useState(0);

    return (
        <div className={css`padding: 8px;`}>
            <h1 className={css`color: white`}>Hello world in Preact</h1>
            <Button variant="contained" onClick={() => setCount((count) => count + 1)}>
                Hello world {count}
            </Button>
        </div>
    );
};

export default App;
