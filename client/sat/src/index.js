import React from "react";
import ReactDOM from "react-dom/client";
import axios from "axios";
import "./index.css";
const normal = 1,
  correct = 2,
  incorrect = 3;

class Square extends React.Component {
  constructor(props) {
    super(props);
    this.handleColorType = this.handleColorType.bind(this);
  }
  handleColorType(type) {
    if (+this.props.value)
      switch (type) {
        case normal:
          return "squareColorAqua";
        case correct:
          return "squareColorGreen";
        case incorrect:
          return "squareColorRed";
        default:
          return "";
      }
  }
  render() {
    return (
      <input
        onChange={this.props.editable ? this.props.onChange : () => null}
        value={+this.props.value || ""}
        className={`square ${
          this.props.editable && this.handleColorType(this.props.type)
        }`}
      />
    );
  }
}

class Board extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      initSudoku: this.props.data[1].map((x) => [...x]),
      finalSudoku: this.props.data[0],
      type: Array(18)
        .fill(1)
        .map(() => new Array(9).fill(normal)),
    };
    this.handleChange = this.handleChange.bind(this);
    this.verifySudoku = this.verifySudoku.bind(this);
    this.resetSudoku = this.resetSudoku.bind(this);
    this.showAnswer = this.showAnswer.bind(this);
  }
  renderSquare(i, pos) {
    return (
      <Square
        value={i}
        onChange={(e) => this.handleChange(e, pos)}
        editable={!this.props.data[1][pos[0]][pos[1]]}
        type={this.state.type[pos[0]][pos[1]]}
        key={pos}
      />
    );
  }

  handleChange(event, pos) {
    if (/^[\d]?$/.test(event.target.value)) {
      let g = this.state.initSudoku.slice();
      g[pos[0]][pos[1]] = +event.target.value;
      if (pos[0] >= 9 && pos[0] < 12 && pos[1] < 3)
        g[pos[0] - 3][pos[1] + 6] = +event.target.value;
      this.setState({
        initSudoku: g,
        type: Array(18)
          .fill(1)
          .map(() => new Array(9).fill(normal)),
      });
    }
  }

  verifySudoku() {
    let type = this.state.type.slice();
    this.state.initSudoku.forEach((x, i) => {
      x.forEach((y, j) => {
        if (y&&!this.props.data[1][i][j]) {
          type[i][j] = y === this.state.finalSudoku[i][j] ? correct : incorrect;
        }
      });
    });
    this.setState({
      type,
    });
  }

  resetSudoku() {
    this.setState({
      initSudoku: this.props.data[1].map(x=>[...x])
    })
  }

  showAnswer() {
    let type = this.state.type.map(x=>x.fill(correct));
    this.setState({
      type,
      initSudoku: this.state.finalSudoku
    })
  }

  render() {
    let grid1 = [],
      grid2 = [];

    for (let i = 0; i < 9; i++) {
      let k = this.state.initSudoku[i].map((item, index) =>
        this.renderSquare(item, [i, index])
      );
      grid1.push(<div>{k}</div>);
    }
    for (let i = 9; i < 18; i++) {
      let k = this.state.initSudoku[i].map((item, index) =>
        this.renderSquare(item, [i, index])
      );
      grid2.push(<div>{k}</div>);
    }
    return (
      <div className="sudokuBox">
        <div className="sudoku sudoku1">{grid1}</div>
        <div className="sudoku sudoku2">{grid2}</div>
      </div>
    );
  }
}
class Solve extends React.Component {
  constructor(props) {
    super(props);
    this.inputFileChange = this.inputFileChange.bind(this);
    this.state = {
      isGetting: false,
      file: "",
    };
  }
  inputFileChange(event) {
    this.setState({
      isGetting: true,
      file: event.target.files[0].name,
    });
    axios
      .get("/api/solve", {
        params: {
          file: event.target.files[0].name.slice(0, -4),
          isOptimize: +this.props.isOptimize,
        },
      })
      .then((res) => {
        alert(res.data.msg);
        this.props.showRes(res.data.file);
      })
      .catch((e) => {
        alert("超时！");
        console.log(e);
      })
      .finally(() => {
        this.setState({
          isGetting: false,
          file: "",
        });
        this.$inputFile.value = "";
      });
  }
  render() {
    return (
      <div>
        <input
          type="file"
          style={{ display: "none" }}
          ref={(ele) => (this.$inputFile = ele)}
          onChange={this.inputFileChange}
          accept=".cnf"
        />
        <button
          className="btn"
          onClick={() => this.$inputFile.click()}
          disabled={this.state.isGetting}
        >
          {this.state.isGetting
            ? "正在求解" + this.state.file
            : "选择文件并求解"}
        </button>
      </div>
    );
  }
}
class Game extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      isOptimize: false,
      data: Array(2)
        .fill(0)
        .map((item) => new Array(18).fill(0).map(() => new Array(9).fill(0))),
      res:""
    };
    this.getSudoku = this.getSudoku.bind(this);
    this.showRes = this.showRes.bind(this);
  }
  async getSudoku() {
    let res = await axios
      .get("/api/sudoku", {
        params: {
          isOptimize: +this.state.isOptimize,
        },
      })
      .catch((e) => {
        alert("超时！");
        console.log(e);
      });
    let m = res.data.msg.pop();
    let d = res.data.msg.map((item) =>
      item.split(" |").map((item1) => item1.split(" ").map((x) => +x))
    );
    this.setState(
      {
        data: d,
      },
      () => alert(m)
    );
  }

  showRes(res){
    this.setState({
      res
    })
  }

  render() {
    return (
      <div className="game">
        <div className="game-board">
          <Board data={this.state.data} key={this.state.data} ref={(ele)=>this.$board = ele}/>
        </div>
        <div className="game-info">
          <button
            className="btn"
            onClick={() => {
              this.setState({
                isOptimize: !this.state.isOptimize,
              });
            }}
          >
            {this.state.isOptimize ? "当前状态：已优化" : "当前状态：未优化"}
          </button>
          <Solve isOptimize={this.state.isOptimize} showRes={this.showRes}/>
          <button className="btn" onClick={this.getSudoku}>
            {"生成数独"}
          </button>
          <button className="btn" onClick={()=>this.$board.verifySudoku()}>验证答案</button>
          <button className="btn" onClick={()=>this.$board.resetSudoku()}>重置数独</button>
          <button className="btn" onClick={()=>this.$board.showAnswer()}>显示答案</button>
        </div>
        <div className="res">{this.state.res}</div>
      </div>
    );
  }
}

// ========================================

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(<Game />);
