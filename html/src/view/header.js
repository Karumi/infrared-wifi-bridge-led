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
                    <div className="navbar-item" >
                        <div className="logo small" />
                    </div>                 
                </div>                
            </nav>
        )
    }
}
