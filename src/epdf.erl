-module(epdf).

-export([from_html/2]).

-on_load(init/0).

init() ->
    SoName = case code:priv_dir(epdf) of
        {error, bad_name} ->
            case filelib:is_dir(filename:join(["..", priv])) of
                true ->
                    filename:join(["..", priv, epdf]);
                _ ->
                    filename:join([priv, epdf])
            end;
        Dir ->
            filename:join(Dir, epdf)
    end,
    erlang:load_nif(SoName, 0).


from_html(_LoadPath, _SavePath) ->
    erlang:nif_error({error, not_loaded}).
