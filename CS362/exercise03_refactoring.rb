class IceBoxRefrigerator

  attr_reader :height, :width, :depth

  def initialize(height = 0, width = 0, depth = 0)
    @height = height
    @width = width
    @depth = depth
  end

  def add(beverage)
  end
end

class BrokenRefrigerator

  attr_reader :height, :width, :depth

  def initialize(height = 0, width = 0, depth = 0)
    @height = height
    @width = width
    @depth = depth
  end

  def add(beverage)
  end

end

class RootBeer; end
class Kombucha; end

class Kitchen

  def initialize
    @refrigerator = IceBoxRefrigerator.new
  end

  def add_beer_to_refrigerator
    @refrigerator.add( RootBeer.new )
  end

  def to_s
    fridge_volume = @refrigerator.height * @refrigerator.width * @refrigerator.depth
    fridge_volume_gallons = fridge_volume * 7.48052
    "Kitchen. Fridge volume = #{fridge_volume_gallons} gallons"
  end

end

# Current usage example:

kitchen = Kitchen.new
kitchen.add_beer_to_refrigerator
puts kitchen

# kitchen has an IceBoxRefrigerator with a RootBeer in it.

# Problem: you can't create a kitchen with a different fridge, and you can't
#          add any beverage other than beer.

# Goal: Create a kitchen with a BrokenRefrigerator, and add Kombucha to it.

