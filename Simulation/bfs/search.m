function [ came_from, steps ] = search( graph, start, target )
%SEARCH Summary of this function goes here
%   Detailed explanation goes here
    frontier = Queue;
    frontier.put(start);
    came_from = containers.Map;
    steps = {start};
    came_from(start) = NaN;
    while ~frontier.empty()
        current = frontier.get();
        neighbours = graph.neighbours(current);
        for i=1:length(neighbours)
            next = neighbours{i};
            if ~came_from.isKey(next)
                steps{end + 1} = next; %#ok<AGROW>
                came_from(next) = current;
                if strcmp(next, target)
                    break
                end
                frontier.put(next);
            end
        end
    end
end

