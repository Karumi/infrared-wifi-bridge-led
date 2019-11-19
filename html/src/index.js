import "./style.css";
import "./bulma.css";
import { Component, h, render } from 'preact';
import Header from './view/header'
import WifiPanel from './view/wifi'
import BoardPanel from './view/board'
import CodesPanel from './view/codes'
import Message from './view/message';
import { store } from './redux/store'
import { startWs } from './redux/ws'

class App extends Component {

	render() {
		const state = store.getState();
		return (
			<div id="app" >
				<Header />
				<Message title="Board restart" message="The board is restarting and connecting to the new WiFi network." visible={state.restart} />
				<BoardPanel />
				<div class="columns">
					<div class="column is-half">
						<WifiPanel />
					</div>
					<div class="column is-half">
						<CodesPanel />
					</div>
				</div>
			</div>
		)
	}
}

const display = () => {
	render(<App />, document.body);
}

store.subscribe(display);

display();
startWs();