classdef Queue < handle
    properties
        container = {};
    end
    methods
        function r = empty(self)
            r = isempty(self.container);
        end
        function put(self, obj)
            self.container{end + 1} = obj;  
        end
        function r = get(self)
            r = self.container{1};
            self.container = self.container(2:end);
        end
    end
end
