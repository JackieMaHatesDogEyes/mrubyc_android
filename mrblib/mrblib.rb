class Array

  def each
    idx = 0
    while idx < length
      yield self[idx]
      idx += 1
    end
    self
  end

end

class Fixnum
  def times
    i = 0
    while i < self
      yield i
      i += 1
    end
    self
  end

  
end
