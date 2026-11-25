def add_item(name: str, cart: list[str] = []) -> list[str]:
    cart.append(name)
    return cart
