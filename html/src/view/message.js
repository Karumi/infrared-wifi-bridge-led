import { Component, h, render } from 'preact';
import {
    boardRestart
} from '../redux/action'
import { store } from '../redux/store'

export default class Message extends Component {

    constructor(props) {
        super(props);
    }

    closeMessage() {
        store.dispatch(boardRestart(false));
    }

    render(props) {
        return (
            <section style={{ display: props.visible ? 'block' : 'none' }} class="section">
                <article class="message is-small is-warning">
                    <div class="message-header">
                        <p>{props.title}</p>
                        <button onClick={this.closeMessage.bind(this)} class="delete is-small" aria-label="delete"></button>
                    </div>
                    <div class="message-body">
                        {props.message}
                    </div>
                </article>
            </section>
        )
    }
}