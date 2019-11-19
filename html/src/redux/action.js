export const UPDATE_SSID = 'update_ssid';
export const ADD_CODE = 'add_code';
export const UPDATE_RELAY = 'update_relay';
export const CLEAN_CODES = 'clean_codes';
export const BOARD_RESTART = 'board_restart';

export const updateSsid = ssid => {
    return {
        type: UPDATE_SSID,
        ssid
    };
};

export const addCode = code => {
    return {
        type: ADD_CODE,
        code
    };
};

export const updateRelay = (relay, boardName) => {
    return {
        type: UPDATE_RELAY,
        relay,
        boardName
    };
};

export const cleanCodes = () => {
    return {
        type: CLEAN_CODES
    };
};

export const boardRestart = (restart) => {
    return {
        type: BOARD_RESTART,
        restart
    };
};