function [ path ] = graph_travel_all( graph, start )
%GRAPH_TRAVEL_ALL Summary of this function goes here
%   Detailed explanation goes here
    make_acyclic(graph, start);
    
    for i = 1:length(graph.detatched_edges)
        graph.edge_attach(graph.detatched_edges{i})
    end
end

function make_acyclic(graph, start)
    frontier = java.util.LinkedList;
    frontier.add(start);
    came_from = cell(1, length(graph.nodes));
    came_from{start} = 0;
    
    while frontier.size() ~= 0
        current = frontier.pop();
        edges = graph.edges{current};
        
        for i=1:length(edges)
            [next, ~] = edges{i}.get_arc_from(current);
            if isempty(came_from{next})
                came_from{next} = edges{i};
                frontier.push(next);
            else
                backtrace_edge = came_from{current};
                backtrace = came_from{current}.get_arc_from(current);
                while (backtrace ~= start && graph.node_order(backtrace) < 3)
                    backtrace_edge = came_from{backtrace};
                    backtrace = backtrace_edge.get_arc_from(backtrace);
                end
                graph.edge_remove(backtrace_edge);
            end
        end
    end
end

