import { Component, h, render } from 'preact';
import { store } from '../redux/store';
import { cleanCodes } from '../redux/action';
import { sendCodes } from '../redux/ws';

const CheckBox = ({ name, onChange }) => (
    <input type="checkbox" name={name} onChange={onChange} />
);

export default class CodesPanel extends Component {

    constructor(props) {
        super(props);

        this.state = {
            checkedCodes: []
        };

        this.getState = this.getState.bind(this);
    }

    getState() {
        const state = store.getState();
        return { ...state, ...this.state }
    }

    sendCodes() {
        cleanCodes();
        sendCodes(this.state.checkedCodes);
    }

    checkCode(event) {
        const checkedCode = event.target.name
        const checked = event.target.checked;
        const currentState = this.state;

        if (checked) {
            this.setState({ checkedCodes: [...currentState.checkedCodes, checkedCode] })
        } else {
            this.setState({ checkedCodes: currentState.checkedCodes.filter(code => code != checkedCode) })
        }
    }

    cleanCodes() {
        store.dispatch(cleanCodes());
    }

    render() {
        const state = this.getState();
        return (
            <section class="section">
                <div style="max-width: 350px;" class="container">
                    <div style="height: 400px;" class="panel">
                        <div class="level panel-heading is-mobile">
                            <div class="level-left">
                                <div class="level-item">
                                    <p class="is-size-6">
                                        <strong>Press the remote control button</strong>
                                    </p>
                                </div>
                            </div>
                            <div class="level-right">
                                <div class="level-item">
                                    <a class="button is-small" onClick={this.cleanCodes}>Clean</a>
                                </div>
                            </div>
                        </div>

                        {state.codes.length == 0 ?
                            <div style="margin-bottom: 24px" class="container full-height">
                                <div class="notification">
                                    <p class="title is-6">There aren't codes</p>
                                    <p class="is-size-7">Press the remote control button to save the infrared codes to turn on/off the lights.</p>
                                </div>
                            </div> :
                            <div style="max-height: 400px; overflow-y: scroll;" class="panel">
                                {state.codes.map(code =>
                                    <label class="panel-block">
                                        <CheckBox name={code} onChange={this.checkCode.bind(this)} />
                                        {code}
                                    </label>
                                )}
                            </div>
                        }
                        <button class="button is-primary is-outlined is-fullwidth" onClick={this.sendCodes.bind(this)}>
                            Save codes
                        </button>
                    </div>
                </div>
            </section>
        )
    }
}