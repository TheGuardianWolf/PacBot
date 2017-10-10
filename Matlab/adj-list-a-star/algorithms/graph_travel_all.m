function [ path ] = graph_travel_all( graph, start )
%GRAPH_TRAVEL_ALL Summary of this function goes here
%   Detailed explanation goes here
    make_acyclic(graph, start);
    
    for i = 1:length(graph.detatched_edges)
        graph.edge_attach(graph.detatched_edges{i})
    end
end

function visited = make_acyclic(graph, start)
    % Modified DFS to break cyclic branches at immediate root
    % Holy crap, this is so inefficient but it works, wow.
    node_orders = zeros(1, length(graph.nodes));
    for i = 1:length(graph.nodes);
        node_orders(i) = graph.node_order(graph.nodes{i}.id);
    end
    frontier = java.util.LinkedList;
    frontier.push(start);
    search_steps_size = 1;
    search_steps = start;
    came_from = cell(1, length(graph.nodes));
    came_from{start} = 0;
    
    while frontier.size() ~= 0
        current = frontier.peek();
        edges = graph.edges{current};
        
        for i=1:length(edges)
            [next, ~] = edges{i}.get_arc_from(current);
            if isempty(came_from{next})
                search_steps_size = search_steps_size + 1;
                search_steps(search_steps_size) = next; %#ok<AGROW>
                came_from{next} = edges{i};
                frontier.push(next);
                break
            else
                if (current ~= start) % It can never be a cycle if you're on start, it's always a cycle if you're going to start, unless you came from start
                    if (came_from{current}.get_arc_from(current) ~= next) && (next == start || came_from{next}.get_arc_from(next) ~= current)
                        if (node_orders(frontier.peek()) >= 3)
                            backtrack_edge = edges{i};
                            graph.edge_remove(backtrack_edge);
                        else
                            while(node_orders(frontier.peek()) < 3)
                                search_steps_size = search_steps_size - 1;
                                backtrack = frontier.pop();
                                backtrack_edge = came_from{backtrack};
                                came_from{backtrack} = [];
                            end
                            graph.edge_remove(backtrack_edge);
                            break
                        end               
                    end
                end
            end
            if i == length(edges)
                frontier.pop();
            end
        end
    end
    visited = search_steps;
end

function path = make_path(graph, start)
    path = cell(1, ceil(length(graph.nodes) * 1.5));
    % Modified BFS to repath shorter branches
    frontier = java.util.LinkedList;
    frontier.add(start);
    came_from = cell(1, length(graph.nodes));
    came_from{start} = 0;
    
    while frontier.size() ~= 0
        current = frontier.remove();
        edges = graph.edges{current};
        
        % if current's order is 1.... backtrack and insert reverse path
        
        for i=1:length(edges)
            [next, ~] = edges{i}.get_arc_from(current);
            if isempty(came_from{next})
                came_from{next} = edges{i};
                frontier.add(next);
            end
        end
    end
end

