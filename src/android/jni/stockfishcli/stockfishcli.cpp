#include <stockfishcli.h>

#include <iostream>
#include <sstream>
#include <string>

#include <evaluate.h>
#include <movegen.h>
#include <position.h>
#include <search.h>
#include <thread.h>
#include <timeman.h>
#include <uci.h>

using namespace std;

extern void benchmark(const Position& pos, istream& is);

namespace stockfishcli
{
  // FEN strings of the initial positions
  const string StartFENs[VARIANT_NB] = {
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#ifdef ANTI
  ,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#endif
#ifdef ATOMIC
  ,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#endif
#ifdef CRAZYHOUSE
  ,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR[] w KQkq - 0 1"
#endif
#ifdef HORDE
  ,"rnbqkbnr/pppppppp/8/1PP2PP1/PPPPPPPP/PPPPPPPP/PPPPPPPP/PPPPPPPP w kq - 0 1"
#endif
#ifdef KOTH
  ,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#endif
#ifdef RACE
  ,"8/8/8/8/8/8/krbnNBRK/qrbnNBRQ w - - 0 1"
#endif
#ifdef RELAY
  ,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#endif
#ifdef THREECHECK
  ,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 3+3 0 1"
#endif
  };

  // A list to keep track of the position states along the setup moves (from the
  // start position to the position just before the search starts). Needed by
  // 'draw by repetition' detection.
  StateListPtr States(new std::deque<StateInfo>(1));


  // position() is called when engine receives the "position" UCI command.
  // The function sets up the position described in the given FEN string ("fen")
  // or the starting position ("startpos") and then makes the moves given in the
  // following move list ("moves").

  void position(Position& pos, istringstream& is) {

    Move m;
    string token, fen;

    Variant variant = UCI::variant_from_name(Options["UCI_Variant"]);

    is >> token;
    if (token == "startpos")
    {
        fen = StartFENs[variant];
        is >> token; // Consume "moves" token if any
    }
    else if (token == "fen")
        while (is >> token && token != "moves")
            fen += token + " ";
    else
        return;

    States = StateListPtr(new std::deque<StateInfo>(1));
    pos.set(fen, Options["UCI_Chess960"], variant, &States->back(), Threads.main());

    // Parse move list (if any)
    while (is >> token && (m = UCI::to_move(pos, token)) != MOVE_NONE)
    {
        States->push_back(StateInfo());
        pos.do_move(m, States->back());
    }
  }


  // setoption() is called when engine receives the "setoption" UCI command. The
  // function updates the UCI option ("name") to the given value ("value").

  void setoption(istringstream& is) {

    string token, name, value;
    is >> token; // Consume "name" token

    // Read option name (can contain spaces)
    while (is >> token && token != "value")
        name += string(" ", name.empty() ? 0 : 1) + token;

    // Read option value (can contain spaces)
    while (is >> token)
        value += string(" ", value.empty() ? 0 : 1) + token;

    if (Options.count(name))
    {
        Options[name] = value;
        if (name == "UCI_Variant") {
            Variant variant = UCI::variant_from_name(value);
            sync_cout << "info string variant " << (string)Options["UCI_Variant"] << " startpos " << StartFENs[variant] << sync_endl;
        }
    }
    else
        sync_cout << "No such option: " << name << sync_endl;
  }


  // go() is called when engine receives the "go" UCI command. The function sets
  // the thinking time and other parameters from the input string, then starts
  // the search.

  void go(Position& pos, istringstream& is) {

    Search::LimitsType limits;
    string token;

    limits.startTime = now(); // As early as possible!

    while (is >> token)
        if (token == "searchmoves")
            while (is >> token)
                limits.searchmoves.push_back(UCI::to_move(pos, token));

        else if (token == "wtime")     is >> limits.time[WHITE];
        else if (token == "btime")     is >> limits.time[BLACK];
        else if (token == "winc")      is >> limits.inc[WHITE];
        else if (token == "binc")      is >> limits.inc[BLACK];
        else if (token == "movestogo") is >> limits.movestogo;
        else if (token == "depth")     is >> limits.depth;
        else if (token == "nodes")     is >> limits.nodes;
        else if (token == "movetime")  is >> limits.movetime;
        else if (token == "mate")      is >> limits.mate;
        else if (token == "infinite")  limits.infinite = 1;
        else if (token == "ponder")    limits.ponder = 1;

    Threads.start_thinking(pos, States, limits);
  }

  Position pos;

  void command(const std::string& cmd) {
    std::string token;
    std::istringstream is(cmd);

    token.clear(); // getline() could return empty or blank line
    is >> skipws >> token;

    // The GUI sends 'ponderhit' to tell us to ponder on the same move the
    // opponent has played. In case Signals.stopOnPonderhit is set we are
    // waiting for 'ponderhit' to stop the search (for instance because we
    // already ran out of time), otherwise we should continue searching but
    // switching from pondering to normal search.
    if (    token == "quit"
        ||  token == "stop"
        || (token == "ponderhit" && Search::Signals.stopOnPonderhit))
    {
      Search::Signals.stop = true;
      Threads.main()->start_searching(true); // Could be sleeping
    }
    else if (token == "ponderhit")
      Search::Limits.ponder = 0; // Switch to normal search

    else if (token == "uci")
      sync_cout << "id name " << engine_info(true)
        << "\n"       << Options
        << "\nuciok"  << sync_endl;

    else if (token == "ucinewgame")
    {
      Search::clear();
      Time.availableNodes = 0;
    }
    else if (token == "isready")    sync_cout << "readyok" << sync_endl;
    else if (token == "go")         go(pos, is);
    else if (token == "position")   position(pos, is);
    else if (token == "setoption")  setoption(is);

    // Additional custom non-UCI commands, useful for debugging
    else if (token == "flip")       pos.flip();
    else if (token == "bench")      benchmark(pos, is);
    else if (token == "d")          sync_cout << pos << sync_endl;
    else if (token == "eval")       sync_cout << Eval::trace(pos) << sync_endl;
    else if (token == "perft")
    {
      int depth;
      stringstream ss;

      is >> depth;
      ss << Options["Hash"]    << " "
        << Options["Threads"] << " " << depth << " current perft";

      benchmark(pos, ss);
    }
    else
      sync_cout << "Unknown command: " << cmd << sync_endl;
  }

}
