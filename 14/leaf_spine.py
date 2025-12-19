from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.log import setLogLevel
from mininet.node import OVSSwitch


class LeafSpineTopo(Topo):
    def build(self):

        # Spine switches
        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')

        # Leaf switches
        l1 = self.addSwitch('l1')
        l2 = self.addSwitch('l2')

        # Hosts
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        h4 = self.addHost('h4')

        # Host to leaf links
        self.addLink(h1, l1)
        self.addLink(h2, l1)
        self.addLink(h3, l2)
        self.addLink(h4, l2)

        # Leaf to spine links
        self.addLink(l1, s1)
        self.addLink(l1, s2)
        self.addLink(l2, s1)
        self.addLink(l2, s2)


def run():
    topo = LeafSpineTopo()

    net = Mininet(
        topo=topo,
        switch=OVSSwitch,
        controller=None,     
        autoSetMacs=True,
        autoStaticArp=True
    )

    net.start()
    print("*** Leaf-Spine topology running without controller ***")
    CLI(net)
    net.stop()


if __name__ == '__main__':
    setLogLevel('info')
    run()

