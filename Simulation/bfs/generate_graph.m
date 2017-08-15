function graph = generate_graph(map)
    graph = SimpleGraph; 
    map_size = size(map);
    for j = 1:map_size(2)
        for i = 1:map_size(1)
            if map(i, j) == 0
                id = [int2str(i) '.' int2str(j)];
                e = find_edges(map, i, j);
                graph.nodes(id) = e;
                if (i == 1 || j == 1 || i == map_size(1) || j == map_size(2))
                    graph.openings{length(graph.openings) + 1} = id;
                end
            end
        end
    end
end

function edges = find_edges(map, i, j)
    vect = [-1, 0; 0, 1; 1, 0; 0, -1];
    edges = cellstr({});
    for k = 1:length(vect)
        cv = vect(k, :);
        i_test = i + cv(1);
        j_test = j + cv(2);
        try
            s = map(i_test, j_test);
            if s == 0
                edges{end + 1} = [int2str(i_test) '.' int2str(j_test)]; %#ok<AGROW>
            end
        catch err
            if (strcmp(err.identifier,'MATLAB:badsubscript'))
                continue
            else
                rethrow(err)
            end
        end
    end
end
