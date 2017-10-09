function [ eulerian_graph ] = graph_make_eulerian( graph )
%GRAPH_MAKE_EULERIAN Summary of this function goes here
%   Detailed explanation goes here
    odd_nodes = [];
%     odd_intersections = [];
%     dead_ends = [];
    for i = 1:length(graph.nodes)
        if graph.node_is_odd(i)
            odd_nodes(end + 1) = i; %#ok<AGROW>
%         if graph.node_order(i) == 1
%             dead_ends(end + 1) = i; %#ok<AGROW>
%         elseif graph.node_order(i) == 3
%             odd_intersections(end + 1) = i; %#ok<AGROW>
        end
    end
    
%     for i = 1:length(dead_ends)
%         edges = graph.edges{dead_ends(i)};
%         new_edge = GraphEdge( ...
%             graph.unique_edges + 1, ...
%             edges{1}.a1(1), edges{1}.a1(2), ...
%             edges{1}.a2(1), edges{1}.a2(2));
%         graph.edges{dead_ends(i)}{2} = new_edge;
%         graph.unique_edges = graph.unique_edges + 1;
%     end

    odd_pairings = {};
    paired = zeros(1, length(odd_nodes));
    for i = 1:length(odd_nodes)
        if paired(i) == 0
            smallest_cost = Inf;
            index = 0;
            possible_pairs = cell(1, length(odd_nodes - i));
            for j = 1:length(possible_pairs)
                if paired(j+i) == 0
                    [path, ~] = graph_astar_inertia(graph, odd_nodes{i}, odd_nodes{j});
                    possible_pairs{j} = path;
                    if length(path) < smallest_cost
                        smallest_cost = length(path);
                        index = j;
                    end
                end
            end
            
            odd_pairings{end + 1} = possible_pairs{index}; %#ok<AGROW>
            paired(i) = 1;
            paired(index + i) = 1;
        end
    end

end

