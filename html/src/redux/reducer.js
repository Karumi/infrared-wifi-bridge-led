import {
    UPDATE_SSID,
    UPDATE_RELAY,
    ADD_CODE,
    CLEAN_CODES,
    BOARD_RESTART
} from './action'

const INITIAL_STATE = {
    boardName: "No Board connected",
    ssid: "",
    relay: false,
    codes: [],
    restart: false
};

export const reducer = (state = INITIAL_STATE, action) => {
    switch (action.type) {
        case UPDATE_RELAY:
            return {
                ...state,
                relay: action.relay,
                boardName: action.boardName
            };
        case UPDATE_SSID:
            return { ...state, ssid: action.ssid };
        case ADD_CODE:
            var newState = state;
            if (!state.codes.includes(action.code)) {
                newState = { ...state, codes: [...state.codes, action.code] };
            }
            return newState;
        case CLEAN_CODES:
            return { ...state, codes: [] }
        case BOARD_RESTART:
            return { ...state, restart: action.restart }
        default:
            return state;
    }
};
