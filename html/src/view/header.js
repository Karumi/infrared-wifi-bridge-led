import { Component, h, render } from 'preact';

export default class Header extends Component {

    constructor(props) {
        super(props);

        this.state = {
            isNavActive: false
        };
    }

    onNavBurgerClick() {
        const isNavActive = this.state.isNavActive;
        this.setState({ isNavActive: !isNavActive });
    }

    render(props, state) {
        return (
            <nav class="navbar" role="navigation" aria-label="main navigation">
                <div class="navbar-brand">
                    <a class="navbar-item" href="https://bulma.io">
                        <img src="https://bulma.io/images/bulma-logo.png" alt="TV" width="112" height="28" />
                    </a>

                    <a role="button"
                        className={state.isNavActive ? "navbar-burger is-active" : "navbar-burger"}
                        onClick={this.onNavBurgerClick.bind(this)}
                        aria-label="menu"
                        aria-expanded="false">
                        <span aria-hidden="true"></span>
                        <span aria-hidden="true"></span>
                        <span aria-hidden="true"></span>
                    </a>
                </div>
                <div className={state.isNavActive ? "navbar-menu is-active" : "navbar-menu"}>
                    <div class="navbar-start">
                        <a class="navbar-item">
                            WiFi
                        </a>
                        <a class="navbar-item">
                            Remote Control
                        </a>
                    </div>
                </div>
            </nav>
        )
    }
}
