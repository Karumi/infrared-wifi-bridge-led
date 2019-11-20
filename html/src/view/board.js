import { Component, h, render } from 'preact';
import { store } from '../redux/store'
import { toggleRelay } from '../redux/ws'

export default class BoardPanel extends Component {

    render() {
        const state = store.getState();
        return (
            <div style="max-width: 350px; margin-top: 24px;" class="level is-mobile container">
                <p class="level-item has-text-centered"><strong>{state.boardName}</strong></p>
                <p class="level-item has-text-centered">
                    {state.relay ?
                        <a class="button is-light" onClick={e => {
                            toggleRelay()
                        }}>Relay Off</a> :
                        <a class="button is-warning" onClick={e => {
                            toggleRelay()
                        }}>Relay On</a>}
                </p>
            </div>
        )
    }
}
