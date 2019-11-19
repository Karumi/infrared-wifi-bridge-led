import { Component, h, render } from 'preact';
import { sendWifi } from '../redux/ws';

export default class WifiPanel extends Component {

    constructor(props) {
        super(props);
        this.state = {
            ssid: "",
            password: "",
            passwordError: false,
            disableSaveButton: true
        };
    }

    onSaveClick() {
        if (!this.state.disableSaveButton) {
            sendWifi(this.state.ssid, this.state.password);
            this.setState({ ...this.state, ssid: "", password: "" })
        }
    }

    validPassword(password) {
        return password.length < 8;
    }

    ssidChange(event) {
        this.setState({ ...this.state, ssid: event.target.value });
    }

    passwordChange(event) {
        const password = event.target.value;
        const error = this.validPassword(password);
        this.setState({
            ...this.state,
            password: password,
            passwordError: error,
            disableSaveButton: error
        });
    }

    render(props, state) {
        return (
            <section class="section">
                <div style="max-width: 350px;" class="container">
                    <div class="field is-centered">
                        <h1 style="text-align: center;" class="title">Wifi Configuration</h1>
                    </div>
                    <div class="field">
                        <div class="control">
                            <input value={state.ssid} onInput={this.ssidChange.bind(this)}
                                class="input" type="text" placeholder="SSID" />
                        </div>
                    </div>
                    <div class="field">
                        <div class="control">
                            <input value={state.password} onInput={this.passwordChange.bind(this)}
                                className={state.passwordError ? "input is-danger" : "input"} type="password" placeholder="Password" />
                        </div>

                        <p style={{
                            visibility: state.passwordError ? 'visible' : 'hidden'
                        }} class="help is-danger">This password is invalid, at least 8 characters</p>
                    </div>
                    <a onClick={this.onSaveClick.bind(this)}
                        class="button is-primary is-fullwidth" disabled={state.disableSaveButton}>Save</a>
                </div>
            </section>
        )
    }
}