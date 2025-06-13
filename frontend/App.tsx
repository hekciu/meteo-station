import React, {
    useState,
    useEffect,
} from 'react';
import type {PropsWithChildren} from 'react';
import {
    SafeAreaView,
    View,
    TextInput,
    Button,
    Text,
    StyleSheet,
} from 'react-native';

import Settings from './components/Settings';
import Measurements from './components/Measurements';

import {
  Colors,
  DebugInstructions,
  Header,
  LearnMoreLinks,
  ReloadInstructions,
} from 'react-native/Libraries/NewAppScreen';


interface Pms5003Data {
    pm10Standard: number,
    pm25Standard: number,
    pm100Standard: number,
    deviceName: string,
    deviceTimestamp: number,
};


function App(): JSX.Element {
    const [serverUrl, setServerUrl] = useState<string>();
    const [showSettings, setShowSettings] = useState(false);


    useEffect(() => {
       // const data =
    }, [serverUrl]);

    return (
        <SafeAreaView style={{position: 'relative'}}>
            <View style={styles.button}>
                <Button title='settings' onPress={() => setShowSettings(!showSettings)}/>
            </View>
                {showSettings ?
                    <Settings
                        serverUrl={serverUrl || ''}
                        setServerUrl={setServerUrl}
                    /> :
                <Measurements/>}
        </SafeAreaView>
    );
}

const styles = StyleSheet.create({
    button: {
        position: 'absolute',
        top: 5,
        right: 5,
        display: 'flex',
        backgroundColor: 'green',
        zIndex: 100,
    },    
});

export default App;
