function [ path ] = graph_travel_all( graph, start )
%GRAPH_TRAVEL_ALL Summary of this function goes here
%   Detailed explanation goes here
    [edge_priority, ~] = make_acyclic(graph, start);
    [path, path_size] = make_path(graph, edge_priority, start);
    
    path = path(1:path_size);
    
    while(~isempty(graph.detatched_edges))
        graph.edge_attach(graph.detatched_edges{1})
    end
end

function [edge_priority, visited] = make_acyclic(graph, start)
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
    edge_priority = zeros(1, graph.unique_edges);
    
    while frontier.size() ~= 0
        current = frontier.peek();
        edges = graph.edges{current};
        
        for i= 1:length(edges)
            [next, ~] = edges{i}.get_arc_from(current);
            if isempty(came_from{next})
                search_steps_size = search_steps_size + 1;
                search_steps(search_steps_size) = next; %#ok<AGROW>
                came_from{next} = edges{i};
                frontier.push(next);
                break
            else
                % It can never be a cycle if you're on start, it's always a cycle if you're going to start, unless you came from start
                if (current ~= start) && (came_from{current}.get_arc_from(current) ~= next) && (next == start || came_from{next}.get_arc_from(next) ~= current)
                    if (node_orders(frontier.peek()) >= 3)
                        backtrack_edge = edges{i};
                        graph.edge_remove(backtrack_edge);
                    else
                        while(node_orders(frontier.peek()) < 3) % do, while
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
            if i == length(edges)
                branch_length = frontier.size();
                last_node = frontier.pop();
                last_edge = came_from{last_node};
                if last_node ~= start
                    edge_priority(last_edge.id) = branch_length;
                end
            end
        end
    end
    visited = search_steps;
end

function [path, path_size] = make_path(graph, edge_priority, start)
    path_size = 0;
    path = zeros(1, ceil(length(graph.nodes) * 1.5));
    frontier = java.util.LinkedList;
    frontier.push(start);
    came_from = cell(1, length(graph.nodes));
    came_from{start} = 0;
    priority = PriorityQueue(4);
    nodes_visited = 1;
    
    while frontier.size() ~= 0
        current = frontier.peek();
        
        path_size = path_size + 1;
        path(path_size) = current;
        if nodes_visited == length(graph.nodes)
            return
        end
        
        edges = graph.edges{current};
        priority.size = 0;
        for i= 1:length(edges)
            p = edge_priority(edges{i}.id);
            priority.put(p, edges{i}, 0);
        end
        
        while ~priority.empty()
            edge = priority.get();
            [next, ~] = edge.get_arc_from(current);
            if isempty(came_from{next})
                came_from{next} = edges{i};
                frontier.push(next);
                nodes_visited = nodes_visited + 1;
                break
            end
            if priority.empty()
                frontier.pop();
            end
        end
    end
end

